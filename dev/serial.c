#include <stdio.h>
#include <sys/ioctl.h>

#include <dev/serial.h>

#define SERIAL_BASE0 0x3f8
#define SERIAL_BASE1 0x2f8
#define SERIAL_BASE2 0x3e8
#define SERIAL_BASE3 0x2e8
#define SERIAL_SIZE 0x8

#define RECEIVE             0x0
#define TRANSMIT            0x0
#define INTERRUPT_ENABLE    0x1
#define INTERRUPT_ID        0x2
#define LINE_CONTROL        0x3
#define MODEM_CONTROL       0x4
#define LINE_STATUS         0x5
#define MODEM_STATUS        0x6
#define SCRATCH             0x7

#define kI8250LineStatusData 0x01
#define kI8250LineStatusEmpty (1u << 5) // 0x20
#define kI8250LineStatusIdle (1u << 6) // 0x40

struct serial_data {
    uint8_t interrupt_enable;
    uint8_t line_control;
};

static int serial_read(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    // printf("serial read handler\n");
    value->u32 = 0;
    switch (port - SERIAL_BASE0) {
        case INTERRUPT_ENABLE:
            value->access_size = 1;
            value->u8 = ((struct serial_data*)dev->priv_data)->interrupt_enable;
            return 0;
        case LINE_CONTROL:
            value->access_size = 1;
            value->u8 = ((struct serial_data*)dev->priv_data)->line_control;
            return 0;
        case LINE_STATUS:
            value->access_size = 1;
            value->u8 = kI8250LineStatusIdle | kI8250LineStatusEmpty;
            return 0;
        case RECEIVE:
        case INTERRUPT_ID:
        case MODEM_CONTROL:
        case MODEM_STATUS... SCRATCH:
            value->access_size = 1;
            value->u8 = 0; // FIXME: modify here to read from stdin
            return 0;
        default:
            printf("[ERROR] unhandled I8250 read 0x%x", port);
            return 1;
    }
    return 1;
}

static int serial_write(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    // printf("serial write handler 0x%x: %d\n", port, value->access_size);
    switch (port - SERIAL_BASE0) {
        case TRANSMIT:
            for (int i = 0; i < value->access_size; i++) {
                printf("%c", value->buf[i]);
            }
            return 0;
        case INTERRUPT_ENABLE:
            if (value->access_size != 1) {
                // return ZX_ERR_IO_DATA_INTEGRITY;
                return 1;
            }
            ((struct serial_data*)dev->priv_data)->interrupt_enable = value->u8;
            return 0;
        case LINE_CONTROL:
            if (value->access_size != 1) {
                // return ZX_ERR_IO_DATA_INTEGRITY;
                return 1;
            }
            ((struct serial_data*)dev->priv_data)->line_control = value->u8;
            return 0;
        case INTERRUPT_ID:
        case MODEM_CONTROL... SCRATCH:
            return 0;
        default:
            printf("[ERROR] unhandled I8250 write 0x%x", port);
            return 1;
    }
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
