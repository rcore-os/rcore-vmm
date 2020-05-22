#include <stdio.h>
#include <sys/ioctl.h>

#include <dev/io_port.h>

#define CMOS_BASE 0x70
#define CMOS_SIZE 0x2

#define SYSTEM_CTRL_PORT 0x92

#define SERIAL_8250_BASE1 0x3f8
#define SERIAL_8250_BASE2 0x2f8
#define SERIAL_8250_BASE3 0x3e8
#define SERIAL_8250_BASE4 0x2e8
#define SERIAL_8250_SIZE 0x8

#define QEMU_DEBUG_PORT 0x402

static int cmos_io_handler(uint64_t port, struct rvm_io_value *value, bool is_input) {
    printf("CMOS handler\n");
    return 0;
}

static void cmos_init(struct io_port_dev *dev) {
    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        CMOS_BASE,
        CMOS_SIZE,
        (uint64_t)cmos_io_handler,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}

static int serial_8250_io_handler(uint64_t port, struct rvm_io_value *value, bool is_input) {
    printf("i8250 handler\n");
    return 1;
}

static void serial_8250_init(struct io_port_dev *dev) {
    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        SERIAL_8250_BASE1,
        SERIAL_8250_SIZE,
        (uint64_t)serial_8250_io_handler,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}

static bool other_io_handler(uint64_t port, struct rvm_io_value *value, bool is_input) {
    printf("other handler\n");
    switch (port) {
    case SYSTEM_CTRL_PORT: // Fast A20 gate
        return 0;
    default:
        return 1;
    }
}

static void other_init(struct io_port_dev *dev) {
    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        SYSTEM_CTRL_PORT,
        1,
        (uint64_t)other_io_handler,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}

void io_port_init(struct io_port_dev *dev, int rvm_fd, int vmid) {
    dev->rvm_fd = rvm_fd;
    dev->vmid = vmid;
    cmos_init(dev);
    serial_8250_init(dev);
    other_init(dev);
}
