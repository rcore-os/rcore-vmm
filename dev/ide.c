#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <dev/ide.h>

#define IDE_BASE0 0x1f0
#define IDE_BASE1 0x170
#define IDE_SIZE 0x8

#define IDE_DISK_SIZE (10 << 20) // 10MB
#define IDE_SEC_SIZE 512

#define IDE_CMD_READ 0x20
#define IDE_CMD_WRITE 0x30
#define IDE_CMD_IDENTIFY 0xEC

struct ide_data
{
    uint8_t *img;

    uint8_t count;
    uint32_t secno;
    uint8_t cmd;
    uint32_t in_sec_offset;
};

static unsigned int ide_id_data[];

static int ide_read(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value)
{
    // printf("IDE read handler\n");
    struct ide_data *data = (struct ide_data *)dev->priv_data;
    value->u32 = 0;
    switch (port - IDE_BASE0)
    {
    case 7:
        value->u32 = 0x40;
        return 0;
    case 0:
        if (data->cmd == IDE_CMD_IDENTIFY) {
            uint8_t *buf = (uint8_t *)ide_id_data;
            for (int i = 0; i < access_size; i++)
            {
                value->buf[i] = buf[data->in_sec_offset];
                data->in_sec_offset++;
            }
            return 0;
        } else if (data->cmd == IDE_CMD_READ) {
            for (int i = 0; i < access_size; i++)
            {
                value->buf[i] = data->img[data->secno * IDE_SEC_SIZE + data->in_sec_offset];
                data->in_sec_offset++;
                data->secno += data->in_sec_offset / IDE_SEC_SIZE;
                data->in_sec_offset %= IDE_SEC_SIZE;
            }
            return 0;
        }
        printf("unknow IDE read cmd : %d\n", data->cmd);
        return 1;
    default:
        printf("IDE unhandled port read 0x%x\n", port);
        return 1;
    }
    return 1;
}

static int ide_write(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value)
{
    // printf("IDE write handler\n");
    struct ide_data *data = (struct ide_data *)dev->priv_data;
    switch (port - IDE_BASE0)
    {
    case 0:
        if (data->cmd == IDE_CMD_WRITE) {
            for (int i = 0; i < access_size; i++)
            {
                data->img[data->secno * IDE_SEC_SIZE + data->in_sec_offset] = value->buf[i];
                data->in_sec_offset++;
                data->secno += data->in_sec_offset / IDE_SEC_SIZE;
                data->in_sec_offset %= IDE_SEC_SIZE;
            }
            return 0;
        }
        printf("unknow IDE write cmd : %d\n", data->cmd);
        return 1;
    case 2:
        data->count = value->u8;
        return 0;
    case 3:
        data->secno -= data->secno & 0xFF;
        data->secno = data->secno | value->u8;
        return 0;
    case 4:
        data->secno -= ((data->secno >> 8) & 0xFF) << 8;
        data->secno = data->secno | ((uint32_t)value->u8 << 8);
        return 0;
    case 5:
        data->secno -= ((data->secno >> 16) & 0xFF) << 16;
        data->secno = data->secno | ((uint32_t)value->u8 << 16);
        return 0;
    case 6:
        data->secno -= ((data->secno >> 24) & 0xFF) << 24;
        // data->secno = data->secno | (((uint32_t)value->u8 & 0xF) << 24);
        return 0;
    case 7:
        data->cmd = value->u8;
        data->in_sec_offset = 0;
        printf("IDE setup, cmd = 0x%x, secno = %d, count = %d\n", data->cmd, data->secno, data->count);
        return 0;
    default:
        printf("IDE unhandled port write 0x%x with value 0x%x\n", port, value->u8);
        return 1;
    }
    return 1;
}

static struct ide_data IDE_DATA;
static const struct virt_device_ops IDE_OPS = {
    .read = ide_read,
    .write = ide_write,
};

void ide_init(struct virt_device *dev, int rvm_fd, int vmid, const char *ide_img)
{
    dev->rvm_fd = rvm_fd;
    dev->vmid = vmid;
    dev->ops = &IDE_OPS;
    dev->priv_data = &IDE_DATA;

    IDE_DATA.img = (uint8_t *)malloc(IDE_DISK_SIZE);
    int fd = open(ide_img, O_RDONLY);
    if (fd < 0)
    {
        printf("[ERROR] unable to open ide img: %s\n", ide_img);
    }
    else
    {
        int size = read(fd, IDE_DATA.img, IDE_DISK_SIZE);
        printf("[INFO] success to load %d bytes ide data\n", size);
        close(fd);
    }

    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        IDE_BASE0,
        IDE_SIZE,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}

static unsigned int ide_id_data[] = {
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
