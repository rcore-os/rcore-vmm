#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <dev/bios.h>

#define BIOS_BASE 0x00
#define BIOS_SIZE 0x8

struct bios_data {
};

static int bios_read(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value) {
    // printf("BIOS read handler\n");
    value->u32 = 0;
    switch (port - BIOS_BASE) {
        default:
            printf("BIOS unhandled port read 0x%x\n", port);
            return 1;
    }
    return 1;
}

static int bios_write(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value) {
    // printf("BIOS write handler\n");
    switch (port - BIOS_BASE) {
        case 0:
            printf("BIOS port write 0x%x, value = 0x%x, continue\n", port, value->u32);
            return 0;
        default:
            printf("BIOS unhandled port write 0x%x, value = 0x%x\n", port, value->u32);
            return 1;
    }
    return 1;
}

static struct bios_data BIOS_DATA;
static const struct virt_device_ops BIOS_OPS = {
    .read = bios_read,
    .write = bios_write,
};

void bios_init(struct virt_device *dev, int rvm_fd, int vmid) {
    dev->rvm_fd = rvm_fd;
    dev->vmid = vmid;
    dev->ops = &BIOS_OPS;
    dev->priv_data = &BIOS_DATA;

    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        BIOS_BASE,
        BIOS_SIZE,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}
