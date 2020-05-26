#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <dev/lpt.h>

#define LPT_BASE 0x378
#define LPT_SIZE 0x8

struct lpt_data {
};

static int lpt_read(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    // printf("LPT read handler\n");
    value->u32 = 0;
    switch (port - LPT_BASE) {
        case 1:
            value->u8 = 0x80;
            return 0;
        default:
            printf("LPT unhandled port read 0x%x\n", port);
            return 1;
    }
    return 1;
}

static int lpt_write(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    // printf("LPT write handler\n");
    switch (port - LPT_BASE) {
        case 0:
        case 2:
            return 0;
        default:
            printf("LPT unhandled port write 0x%x\n", port);
            return 1;
    }
    return 1;
}

static struct lpt_data LPT_DATA;
static const struct virt_device_ops LPT_OPS = {
    .read = lpt_read,
    .write = lpt_write,
};

void lpt_init(struct virt_device *dev, int rvm_fd, int vmid) {
    dev->rvm_fd = rvm_fd;
    dev->vmid = vmid;
    dev->ops = &LPT_OPS;
    dev->priv_data = &LPT_DATA;

    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        LPT_BASE,
        LPT_SIZE,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}
