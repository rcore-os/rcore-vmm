#ifndef VMM_DEV_IDE_H
#define VMM_DEV_IDE_H

#include <dev/dev.h>

void ide_init(struct virt_device *dev, int rvm_fd, int vmid, const char* ide_img);

#endif // VMM_DEV_IDE_H
