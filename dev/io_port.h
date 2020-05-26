#ifndef VMM_DEV_IO_PORT_H
#define VMM_DEV_IO_PORT_H

#include <dev/dev.h>

void io_port_init(struct virt_device *dev, int rvm_fd, int vmid);

#endif // VMM_DEV_IO_PORT_H
