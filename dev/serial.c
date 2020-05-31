#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

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

#define KB_DATA_SIZE        256

struct serial_data {
    uint8_t interrupt_enable;
    uint8_t line_control;

    uint8_t kb_data[KB_DATA_SIZE];
    int kb_r_pos;
    int kb_w_pos;
};

// return true if has new data
bool check_stdin(struct serial_data* data) {
    uint8_t buf;
    if ((data->kb_w_pos + 1) % KB_DATA_SIZE != data->kb_r_pos) {
        int n = read(0, &buf, 1);
        if (n > 0) {
            data->kb_data[data->kb_w_pos] = buf;
            data->kb_w_pos = (data->kb_w_pos + 1) % KB_DATA_SIZE;
            return true;
        }
    }
    return false;
}

static int serial_read(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value) {
    // printf("serial read handler\n");
    value->u32 = 0;
    struct serial_data* data = (struct serial_data*)dev->priv_data;
    switch (port - SERIAL_BASE0) {
        case INTERRUPT_ENABLE:
            value->u8 = data->interrupt_enable;
            return 0;
        case LINE_CONTROL:
            value->u8 = data->line_control;
            return 0;
        case LINE_STATUS:
            check_stdin(data);
            if (data->kb_r_pos != data->kb_w_pos) {
                value->u8 = kI8250LineStatusIdle | kI8250LineStatusData;
            } else {
                value->u8 = kI8250LineStatusIdle | kI8250LineStatusEmpty;
            }
            return 0;
        case RECEIVE:
            for (int i = 0; i < access_size; i ++) {
                check_stdin(data);
            }
            value->u32 = 0;
            for (int i = 0; i < access_size; i ++) {
                if (data->kb_r_pos != data->kb_w_pos) {
                    value->buf[i] = data->kb_data[data->kb_r_pos];
                    data->kb_r_pos = (data->kb_r_pos + 1) % KB_DATA_SIZE;
                }
            }
            return 0;
        case INTERRUPT_ID:
        case MODEM_CONTROL:
        case MODEM_STATUS... SCRATCH:
            value->u8 = 0; // FIXME: modify here to read from stdin
            return 0;
        default:
            printf("[ERROR] unhandled I8250 read 0x%x", port);
            return 1;
    }
    return 1;
}

static int serial_write(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value) {
    // printf("serial write handler 0x%x: %d\n", port, value->access_size);
    struct serial_data* data = (struct serial_data*)dev->priv_data;
    switch (port - SERIAL_BASE0) {
        case TRANSMIT:
            for (int i = 0; i < access_size; i++) {
                printf("%c", value->buf[i]);
            }
            fflush(stdout);
            return 0;
        case INTERRUPT_ENABLE:
            if (access_size != 1) {
                // return ZX_ERR_IO_DATA_INTEGRITY;
                return 1;
            }
            data->interrupt_enable = value->u8;
            return 0;
        case LINE_CONTROL:
            if (access_size != 1) {
                // return ZX_ERR_IO_DATA_INTEGRITY;
                return 1;
            }
            data->line_control = value->u8;
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
    SERIAL_DATA.kb_r_pos = 0;
    SERIAL_DATA.kb_w_pos = 0;
    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        SERIAL_BASE0,
        SERIAL_SIZE,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}

bool has_new_data(struct virt_device *dev) {
    struct serial_data* data = (struct serial_data*)dev->priv_data;
    return check_stdin(data);
}
