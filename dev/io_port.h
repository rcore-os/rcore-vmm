#ifndef VMM_DEV_IO_PORT_H
#define VMM_DEV_IO_PORT_H

struct io_port_dev {
    int rvm_fd;
    int vmid;
};

extern void io_port_init(struct io_port_dev *dev, int rvm_fd, int vmid);

#endif // VMM_DEV_IO_PORT_H
