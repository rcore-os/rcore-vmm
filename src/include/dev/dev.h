#ifndef VMM_DEV_DEV_H
#define VMM_DEV_DEV_H

#include <rvm.h>

struct virt_device_ops;
struct virt_device {
    int rvm_fd;
    int vmid;
    void *priv_data;

    const struct virt_device_ops *ops;
};

struct virt_device_ops {
#ifdef __amd64
    int (*read)(struct virt_device *dev, uint16_t port, struct rvm_io_value *value);
    int (*write)(struct virt_device *dev, uint16_t port, struct rvm_io_value *value);
#endif
#ifdef __riscv
    int (*memop)(struct virt_device* dev, struct rvm_mmio_value* value);
#endif
};

#endif // VMM_DEV_DEV_H
