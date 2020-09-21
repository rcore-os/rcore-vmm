#ifndef VMM_DEV_LPT_H
#define VMM_DEV_LPT_H

#include <dev/dev.h>

// I dont know this device
void lpt_init(struct virt_device *dev, int rvm_fd, int vmid);

#endif // VMM_DEV_LPT_H
