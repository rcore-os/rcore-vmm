#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <rvm.h>
#include <init.h>


int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

    int fd = open("/dev/rvm", O_RDWR);
    printf("rvm fd = %d\n", fd);
    if (fd < 0) {
        printf("failed to open /dev/rvm: %s\n", strerror(errno));
        return 1;
    }

    int vmid = ioctl(fd, RVM_GUEST_CREATE);
    printf("vmid = %d\n", vmid);
    if (vmid < 0) {
        printf("failed to create guest: %s\n", strerror(errno));
        return 1;
    }

    // if (argc > 1)
    //     img = argv[1];
    // int entry = init_memory_ucore(fd, vmid, img);
    // if (entry < 0)
    //     return 0;
    int vcpu_id;
    struct vm_mem_set* mem_set;
    if(init_vm(fd, vmid, &vcpu_id, &mem_set)){
        return 1;
    }
    for (;;) {
        
        struct rvm_vcpu_resume_args args;
        //printf("arg packet addr = %p\n", (void*)&args);
        args.vcpu_id = (unsigned short) vcpu_id;
        int ret = ioctl(fd, RVM_VCPU_RESUME, &args);
        if (ret < 0) {
            printf("failed to resume vcpu: %s\n", strerror(errno));
            break;
        }
        ret = handle_exit(vcpu_id, &args.packet, mem_set);
        if (ret < 0) {
            printf("failed to handle VM exit (kind = %d): %s\n", args.packet.kind, strerror(errno));
            break;
        }
    }

    close(fd);
    return 0;
}
