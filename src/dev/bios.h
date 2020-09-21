#ifndef VMM_DEV_BIOS_H
#define VMM_DEV_BIOS_H

#include <dev/dev.h>

// fake bios debug port
void bios_init(struct virt_device *dev, int rvm_fd, int vmid);

#endif // VMM_DEV_BIOS_H
