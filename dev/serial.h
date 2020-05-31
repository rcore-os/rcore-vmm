#ifndef VMM_DEV_SERIAL_H
#define VMM_DEV_SERIAL_H

#include <dev/dev.h>

void serial_init(struct virt_device *dev, int rvm_fd, int vmid);

bool has_new_data(struct virt_device *dev);

#endif // VMM_DEV_SERIAL_H
