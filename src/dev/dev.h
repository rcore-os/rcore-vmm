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
    int (*read)(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value);
    int (*write)(struct virt_device *dev, uint64_t port, uint8_t access_size, union rvm_io_value *value);
};

#endif // VMM_DEV_DEV_H
