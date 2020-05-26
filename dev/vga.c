#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <dev/vga.h>

#define VGA_BASE 0x3D4
#define VGA_SIZE 0x8

struct vga_data {
};

static int vga_read(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    // printf("VGA read handler\n");
    value->u32 = 0;
    switch (port - VGA_BASE) {
        case 1:
            return 0;
        default:
            printf("VGA unhandled port read 0x%x\n", port);
            return 1;
    }
    return 1;
}

static int vga_write(struct virt_device *dev, uint64_t port, struct rvm_io_value *value) {
    // printf("VGA write handler\n");
    switch (port - VGA_BASE) {
        case 0:
        case 1:
            return 0;
        default:
            printf("VGA unhandled port write 0x%x\n", port);
            return 1;
    }
    return 1;
}

static struct vga_data VGA_DATA;
static const struct virt_device_ops VGA_OPS = {
    .read = vga_read,
    .write = vga_write,
};

void vga_init(struct virt_device *dev, int rvm_fd, int vmid) {
    dev->rvm_fd = rvm_fd;
    dev->vmid = vmid;
    dev->ops = &VGA_OPS;
    dev->priv_data = &VGA_DATA;

    struct rvm_guest_set_trap_args trap = {
        dev->vmid,
        RVM_TRAP_KIND_IO,
        VGA_BASE,
        VGA_SIZE,
        (uint64_t)dev,
    };
    ioctl(dev->rvm_fd, RVM_GUEST_SET_TRAP, &trap);
}
