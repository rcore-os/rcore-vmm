#include <stdio.h>
#include <sys/ioctl.h>

#include <dev/ide.h>

#define IDE_BASE0 0x1f0
#define IDE_BASE1 0x170
#define IDE_SIZE 0x8

struct ide_data {
};

static int ide_read(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    printf("IDE read handler\n");
    value->u32 = 0x40;
    return 0;
}

static int ide_write(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    printf("IDE write handler\n");
    return 0;
}

static struct ide_data IDE_DATA;
static const struct virt_device_ops IDE_OPS = {
    .read = ide_read,
    .write = ide_write,
};

void ide_init(struct virt_device *dev, int rvm_fd, int vmid) {
    dev->rvm_fd = rvm_fd;
    dev->vmid = vmid;
    dev->ops = &IDE_OPS;
    dev->priv_data = &IDE_DATA;
    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        IDE_BASE0,
        IDE_SIZE,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}
