#ifndef VMM_DEV_IDE_H
#define VMM_DEV_IDE_H

#include <dev/dev.h>

void ide_init(struct virt_device *dev, int rvm_fd, int vmid);

void ide_add_file_img(struct virt_device *dev, const char* ide_file_img);
void ide_add_empty_img(struct virt_device *dev, uint64_t size);

#endif // VMM_DEV_IDE_H
