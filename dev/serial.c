#include <stdio.h>
#include <sys/ioctl.h>

#include <dev/serial.h>

#define SERIAL_BASE0 0x3f8
#define SERIAL_BASE1 0x2f8
#define SERIAL_BASE2 0x3e8
#define SERIAL_BASE3 0x2e8
#define SERIAL_SIZE 0x8

struct serial_data {
};

static int serial_read(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    printf("serial read handler\n");
    return 1;
}

static int serial_write(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    printf("serial write handler\n");
    return 1;
}

static struct serial_data SERIAL_DATA;
static const struct virt_device_ops SERIAL_OPS = {
    .read = serial_read,
    .write = serial_write,
};

void serial_init(struct virt_device *dev, int rvm_fd, int vmid) {
    dev->rvm_fd = rvm_fd;
    dev->vmid = vmid;
    dev->ops = &SERIAL_OPS;
    dev->priv_data = &SERIAL_DATA;
    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        SERIAL_BASE0,
        SERIAL_SIZE,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}
