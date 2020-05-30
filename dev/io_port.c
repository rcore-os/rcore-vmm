#include <stdio.h>
#include <sys/ioctl.h>

#include <dev/io_port.h>

#define CMOS_BASE 0x70
#define CMOS_SIZE 0x2

#define SYSTEM_CTRL_PORT 0x92

#define QEMU_DEBUG_PORT 0x402

struct cmos_data {
};

struct other_data {
};

static int cmos_read(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value) {
    printf("CMOS read handler\n");
    return 0;
}

static int cmos_write(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value) {
    printf("CMOS write handler\n");
    return 0;
}

static int other_read(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value) {
    printf("other read handler\n");
    return 1;
}

static int other_write(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value) {
    printf("other write handler\n");
    switch (port) {
    case SYSTEM_CTRL_PORT: // Fast A20 gate
        return 0;
    default:
        return 1;
    }
}

static struct cmos_data CMOS_DATA;
static struct virt_device_ops CMOS_OPS = {
    .read = cmos_read,
    .write = cmos_write,
};

static void cmos_init(struct virt_device *dev) {
    dev->ops = &CMOS_OPS;
    dev->priv_data = &CMOS_DATA;
    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        CMOS_BASE,
        CMOS_SIZE,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}

static struct other_data OTHER_DATA;
static struct virt_device_ops OTHER_OPS = {
    .read = other_read,
    .write = other_write,
};

static void other_init(struct virt_device *dev) {
    dev->ops = &OTHER_OPS;
    dev->priv_data = &OTHER_DATA;
    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        SYSTEM_CTRL_PORT,
        1,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}

void io_port_init(struct virt_device *dev, int rvm_fd, int vmid) {
    dev->rvm_fd = rvm_fd;
    dev->vmid = vmid;
    cmos_init(dev);
    other_init(dev);
}
