#ifndef VMM_DEV_IO_PORT_H
#define VMM_DEV_IO_PORT_H

#include <rvm.h>

typedef int (*io_handler_t)(uint64_t port, struct rvm_io_value *value, bool is_input);

struct io_port_dev {
    int rvm_fd;
    int vmid;
};

extern void io_port_init(struct io_port_dev *dev, int rvm_fd, int vmid);

#endif // VMM_DEV_IO_PORT_H
