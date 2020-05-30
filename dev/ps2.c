#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <dev/ps2.h>

#define PS2_BASE 0x60
#define PS2_SIZE 0x5

struct ps2_data {
};

static int ps2_read(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    // printf("PS2 read handler\n");
    value->u32 = 0;
    switch (port - PS2_BASE) {
        case 4:
            value->u8 = 0;
            return 0;
        default:
            printf("PS2 unhandled port read 0x%x\n", port);
            return 1;
    }
    return 1;
}

static int ps2_write(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    // printf("PS2 write handler\n");
    switch (port - PS2_BASE) {
        case 0:
        case 4:
            return 0;
        default:
            printf("PS2 unhandled port write 0x%x\n", port);
            return 1;
    }
    return 1;
}

static struct ps2_data PS2_DATA;
static const struct virt_device_ops PS2_OPS = {
    .read = ps2_read,
    .write = ps2_write,
};

void ps2_init(struct virt_device *dev, int rvm_fd, int vmid) {
    dev->rvm_fd = rvm_fd;
    dev->vmid = vmid;
    dev->ops = &PS2_OPS;
    dev->priv_data = &PS2_DATA;

    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        PS2_BASE,
        PS2_SIZE,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}
