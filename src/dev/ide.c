#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <dev/ide.h>

#define IDE_BASE0 0x1f0
#define IDE_BASE1 0x170
#define IDE_PORT_SIZE 0x8

#define IDE_SEC_SIZE 512

#define ISA_DATA                0x00
#define ISA_ERROR               0x01
#define ISA_PRECOMP             0x01
#define ISA_CTRL                0x02
#define ISA_SECCNT              0x02
#define ISA_SECTOR              0x03
#define ISA_CYL_LO              0x04
#define ISA_CYL_HI              0x05
#define ISA_SDH                 0x06
#define ISA_COMMAND             0x07
#define ISA_STATUS              0x07

#define IDE_IDENT_SECTORS       20
#define IDE_IDENT_MODEL         54
#define IDE_IDENT_CAPABILITIES  98
#define IDE_IDENT_CMDSETS       164
#define IDE_IDENT_MAX_LBA       120
#define IDE_IDENT_MAX_LBA_EXT   200

#define IDE_CMD_READ 0x20
#define IDE_CMD_WRITE 0x30
#define IDE_CMD_IDENTIFY 0xEC

struct ide_disk
{
    uint32_t valid;

    uint8_t* img;
    int img_fd;
    uint32_t total_size;

    uint8_t* id_data;
};

struct ide_base_data // for each base port
{
    struct ide_disk disk[2];

    uint8_t count;
    uint32_t disk_id;
    uint32_t secno;
    uint8_t cmd;
    uint32_t in_sec_offset;
};

struct ide_data
{
    struct ide_base_data base[2];
};

static int ide_base_read(uint32_t IOBASE, struct virt_device *dev, struct ide_base_data* base, uint64_t port, uint8_t access_size, union rvm_io_value *value)
{
    // printf("IDE read handler\n");
    value->u32 = 0;
    switch (port - IOBASE)
    {
    case ISA_STATUS:
        if (base->disk[base->disk_id].valid) {
            value->u32 = 0x40;
        } else {
            value->u32 = 0x00;
        }
        return 0;
    case ISA_DATA:
        if (base->cmd == IDE_CMD_IDENTIFY) { // FIXME
            const uint8_t *buf = base->disk[base->disk_id].id_data;
            for (int i = 0; i < access_size; i++)
            {
                value->buf[i] = buf[base->in_sec_offset];
                base->in_sec_offset++;
            }
            return 0;
        } else if (base->cmd == IDE_CMD_READ) {
            for (int i = 0; i < access_size; i++)
            {
                value->buf[i] = base->disk[base->disk_id].img[base->secno * IDE_SEC_SIZE + base->in_sec_offset];
                base->in_sec_offset++;
                base->secno += base->in_sec_offset / IDE_SEC_SIZE;
                base->in_sec_offset %= IDE_SEC_SIZE;
            }
            return 0;
        }
        printf("unknow IDE read cmd : %d\n", base->cmd);
        return 1;
    default:
        printf("IDE unhandled port read 0x%x\n", port);
        return 1;
    }
    return 1;
}

static int ide_base_write(uint32_t IOBASE, struct virt_device *dev, struct ide_base_data* base, uint64_t port, uint8_t access_size, union rvm_io_value *value)
{
    // printf("IDE write handler\n");
    switch (port - IOBASE)
    {
    case ISA_DATA:
        if (base->cmd == IDE_CMD_WRITE) {
            for (int i = 0; i < access_size; i++)
            {
                base->disk[base->disk_id].img[base->secno * IDE_SEC_SIZE + base->in_sec_offset] = value->buf[i];
                base->in_sec_offset++;
                base->secno += base->in_sec_offset / IDE_SEC_SIZE;
                base->in_sec_offset %= IDE_SEC_SIZE;
            }
            return 0;
        }
        printf("unknow IDE write cmd : %d\n", base->cmd);
        return 1;
    case ISA_SECCNT:
        base->count = value->u8;
        return 0;
    case ISA_SECTOR:
        base->secno -= base->secno & 0xFF;
        base->secno = base->secno | value->u8;
        return 0;
    case ISA_CYL_LO:
        base->secno -= ((base->secno >> 8) & 0xFF) << 8;
        base->secno = base->secno | ((uint32_t)value->u8 << 8);
        return 0;
    case ISA_CYL_HI:
        base->secno -= ((base->secno >> 16) & 0xFF) << 16;
        base->secno = base->secno | ((uint32_t)value->u8 << 16);
        return 0;
    case ISA_SDH:
        // 0xE0 | ((ideno & 1) << 4) | ((secno >> 24) & 0xF)
        // 111 disk_id:1 secno:4
        base->secno -= ((base->secno >> 24) & 0xFF) << 24;
        base->secno = base->secno | (((uint32_t)value->u8 & 0xF) << 24);
        base->disk_id = (value->u8 >> 4) & 1;
        return 0;
    case ISA_COMMAND:
        base->cmd = value->u8;
        base->in_sec_offset = 0;
        // printf("IDE setup, base = %d, disk_id = %d, cmd = 0x%x, secno = %d, count = %d\n", IOBASE, base->disk_id, base->cmd, base->secno, base->count);
        return 0;
    default:
        printf("IDE unhandled port write 0x%x with value 0x%x\n", port, value->u8);
        return 1;
    }
    return 1;
}

static int ide_read(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value) {
    if (IDE_BASE0 <= port && port < IDE_BASE0 + IDE_PORT_SIZE) {
        return ide_base_read(IDE_BASE0, dev, &((struct ide_data*)dev->priv_data)->base[0], port, access_size, value);
    } else if (IDE_BASE1 <= port && port < IDE_BASE1 + IDE_PORT_SIZE) {
        return ide_base_read(IDE_BASE1, dev, &((struct ide_data*)dev->priv_data)->base[1], port, access_size, value);
    } else {
        printf("BIOS unhandled port read 0x%x\n", port);
        return 1;
    }
}

static int ide_write(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value) {
    if (IDE_BASE0 <= port && port < IDE_BASE0 + IDE_PORT_SIZE) {
        return ide_base_write(IDE_BASE0, dev, &((struct ide_data*)dev->priv_data)->base[0], port, access_size, value);
    } else if (IDE_BASE1 <= port && port < IDE_BASE1 + IDE_PORT_SIZE) {
        return ide_base_write(IDE_BASE1, dev, &((struct ide_data*)dev->priv_data)->base[1], port, access_size, value);
    } else {
        printf("BIOS unhandled port write 0x%x\n", port);
        return 1;
    }
}

static struct ide_data IDE_DATA;
static const struct virt_device_ops IDE_OPS = {
    .read = ide_read,
    .write = ide_write,
};

void ide_init(struct virt_device *dev, int rvm_fd, int vmid)
{
    dev->rvm_fd = rvm_fd;
    dev->vmid = vmid;
    dev->ops = &IDE_OPS;
    dev->priv_data = &IDE_DATA;

    for (int base_id = 0; base_id < 2; base_id ++) {
        IDE_DATA.base[base_id].count = 0;
        IDE_DATA.base[base_id].disk_id = 0;
        IDE_DATA.base[base_id].secno = 0;
        IDE_DATA.base[base_id].cmd = 0;
        IDE_DATA.base[base_id].in_sec_offset = 0;
        for (int disk_id = 0; disk_id < 2; disk_id ++) {
            IDE_DATA.base[base_id].disk[disk_id].valid = 0;
            IDE_DATA.base[base_id].disk[disk_id].img = NULL;
            IDE_DATA.base[base_id].disk[disk_id].img_fd = -1;
            IDE_DATA.base[base_id].disk[disk_id].total_size = 0;
            IDE_DATA.base[base_id].disk[disk_id].id_data = NULL;
        }
    }

    struct rvm_guest_set_trap_args trap_base0 = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        IDE_BASE0,
        IDE_PORT_SIZE,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap_base0);

    struct rvm_guest_set_trap_args trap_base1 = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        IDE_BASE1,
        IDE_PORT_SIZE,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap_base1);
}

static unsigned int default_ide_id_data[];
void ide_disk_gen_id(struct ide_disk* disk)
{
    uint8_t* ident = (uint8_t *)malloc(IDE_SEC_SIZE);
    memcpy(ident, default_ide_id_data, IDE_SEC_SIZE);

    // sectors
    *(uint32_t*)(ident + IDE_IDENT_MAX_LBA_EXT) = disk->total_size / IDE_SEC_SIZE;
    *(uint32_t*)(ident + IDE_IDENT_MAX_LBA) = disk->total_size / IDE_SEC_SIZE;

    const char* model_name = "RCORE VMM HARDDISK";
    uint8_t* data = ident + IDE_IDENT_MODEL;
    for (int i = 0; i < 40; i ++) {
        data[i] = ' ';
    }
    memcpy(data, model_name, strlen(model_name));
    // swap
    for (int i = 0; i < 40; i += 2) {
        uint8_t t =  data[i]; data[i] = data[i+1]; data[i+1] = t;
    }

    disk->id_data = ident;
}

void ide_add_file_img(struct virt_device *dev, const char* ide_file_img)
{
    struct ide_data* data = (struct ide_data*)dev->priv_data;

    for (int base_id = 0; base_id < 2; base_id ++) {
        for (int disk_id = 0; disk_id < 2; disk_id ++) {
            if (!data->base[base_id].disk[disk_id].valid) {
                data->base[base_id].disk[disk_id].valid = 1;

                struct stat statbuf;
                stat(ide_file_img, &statbuf);
                int file_size = statbuf.st_size;

                int total_size = (file_size + IDE_SEC_SIZE - 1) / IDE_SEC_SIZE * IDE_SEC_SIZE;
                int img_fd = open(ide_file_img, O_RDWR);
                uint8_t* img = NULL;

                if (img_fd < 0)
                {
                    printf("[ERROR] unable to open ide img: %s\n", ide_file_img);
                    return;
                }
                else
                {
                    img = (uint8_t *)mmap(NULL, file_size, PROT_READ | PROT_READ, MAP_PRIVATE, img_fd, 0);
                }
                printf("[INFO] success to map %s to ide base:disk = %d:%d\n", ide_file_img, base_id, disk_id);
                data->base[base_id].disk[disk_id].img = img;
                data->base[base_id].disk[disk_id].img_fd = img_fd;
                data->base[base_id].disk[disk_id].total_size = total_size;
                ide_disk_gen_id(&data->base[base_id].disk[disk_id]);
                return;
            }
        }
    }
    printf("[ERROR] no disk space for ide img: %s\n", ide_file_img);
}
void ide_add_empty_img(struct virt_device *dev, uint64_t size)
{
    struct ide_data* data = (struct ide_data*)dev->priv_data;
    size = (size + IDE_SEC_SIZE - 1) / IDE_SEC_SIZE * IDE_SEC_SIZE;

    for (int base_id = 0; base_id < 2; base_id ++) {
        for (int disk_id = 0; disk_id < 2; disk_id ++) {
            if (!data->base[base_id].disk[disk_id].valid) {
                data->base[base_id].disk[disk_id].valid = 1;
                data->base[base_id].disk[disk_id].img = (uint8_t *)malloc(size);
                data->base[base_id].disk[disk_id].total_size = size;
                ide_disk_gen_id(&data->base[base_id].disk[disk_id]);

                printf("[INFO] success to add empty disk %ld bytes, base:disk = %d:%d\n", size, base_id, disk_id);
                return;
            }
        }
    }
    printf("[ERROR] no disk space for empty ide\n");
}

// from qemu
static unsigned int default_ide_id_data[] = {
    0x90040,
    0x100000,
    0x2007e00,
    0x3f,
    0x0,
    0x3030514d,
    0x31203030,
    0x20202020,
    0x20202020,
    0x20202020,
    0x2000003,
    0x322e0004,
    0x2020352b,
    0x51452020,
    0x20484d55,
    0x44444152,
    0x4b204953,
    0x20202020,
    0x20202020,
    0x20202020,
    0x20202020,
    0x20202020,
    0x20202020,
    0x80102020,
    0xb000001,
    0x2000000,
    0x70200,
    0x100009,
    0x2370003f,
    0x1100000,
    0x2710,
    0x70007,
    0x780003,
    0x780078,
    0x40000078,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x1600f0,
    0x74004021,
    0x40214000,
    0x40003400,
    0x203f,
    0x0,
    0x60010000,
    0x0,
    0x0,
    0x0,
    0x2710,
    0x0,
    0x0,
    0x6000,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x10000,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
};
