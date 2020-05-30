#ifndef VMM_DEV_PS2_H
#define VMM_DEV_PS2_H

#include <dev/dev.h>

// I dont know this device
// "8042" PS/2 Controller ? Enable A20?
void ps2_init(struct virt_device *dev, int rvm_fd, int vmid);

#endif // VMM_DEV_PS2_H
