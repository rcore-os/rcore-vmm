#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <dev/ide.h>

#define IDE_BASE0 0x1f0
#define IDE_BASE1 0x170
#define IDE_SIZE 0x8

#define IDE_DISK_SIZE (10 << 20) // 20MB
#define IDE_SEC_SIZE 512

struct ide_data {
    uint8_t* img;

    uint8_t count;
    uint32_t secno;
    uint8_t cmd;
    uint32_t in_sec_offset;
};

static int ide_read(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    // printf("IDE read handler\n");
    struct ide_data* data = (struct ide_data*)dev->priv_data;
    value->u32 = 0;
    switch (port - IDE_BASE0) {
        case 7:
            value->u32 = 0x40;
            return 0;
        case 0:
            for (int i = 0; i < value->access_size; i ++) {
                value->buf[i] = data->img[data->secno * IDE_SEC_SIZE + data->in_sec_offset];
                data->in_sec_offset ++;
                data->secno += data->in_sec_offset / IDE_SEC_SIZE;
                data->in_sec_offset %= IDE_SEC_SIZE;
            }
            // printf("IDE read data : %d\n", value->access_size);
            return 0;
        default:
            printf("IDE unhandled port read 0x%x\n", port);
            return 1;
    }
    return 1;
}

static int ide_write(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    printf("IDE write handler\n");
    struct ide_data* data = (struct ide_data*)dev->priv_data;
    switch (port - IDE_BASE0) {
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
            data->secno = data->secno | (((uint32_t)value->u8 & 0xF) << 24);
            return 0;
        case 7:
            data->cmd = value->u8;
            data->in_sec_offset = 0;
            printf("IDE setup, cmd = 0x%x, secno = %d, count = %d\n", data->cmd, data->secno, data->count);
            return 0;
        default:
            printf("IDE unhandled port write 0x%x\n", port);
            return 1;
    }
    return 1;
}

static struct ide_data IDE_DATA;
static const struct virt_device_ops IDE_OPS = {
    .read = ide_read,
    .write = ide_write,
};

void ide_init(struct virt_device *dev, int rvm_fd, int vmid, const char* ide_img) {
    dev->rvm_fd = rvm_fd;
    dev->vmid = vmid;
    dev->ops = &IDE_OPS;
    dev->priv_data = &IDE_DATA;

    IDE_DATA.img = (uint8_t*)malloc(IDE_DISK_SIZE);
    int fd = open(ide_img, O_RDONLY);
    if (fd < 0) {
        printf("[ERROR] unable to open ide img: %s\n", ide_img);
    } else {
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
