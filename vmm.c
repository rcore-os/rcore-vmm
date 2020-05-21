#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dev/io_port.h>
#include <rvm.h>

const uint32_t GUEST_RAM_SZ = 16 * 1024 * 1024; // 16 MiB

struct io_port_dev IO_PORT;

void test_hypercall() {
    for (int i = 0;; i++) {
        asm volatile(
            "vmcall"
            :
            : "a"(i),
              "b"(2),
              "c"(3),
              "d"(3),
              "S"(3)
            : "memory");
    }
}

int init_memory(int rvm_fd, int vmid, const char *bios_file) {
    // RAM
    struct rvm_guest_add_memory_region_args region = {vmid, 0, GUEST_RAM_SZ};
    char *ram_ptr = (char *)(intptr_t)ioctl(rvm_fd, RVM_GUEST_ADD_MEMORY_REGION, &region);

    int fd = open(bios_file, O_RDONLY);
    if (fd < 0)
        return -1;

    struct stat statbuf;
    stat(bios_file, &statbuf);
    int bios_size = statbuf.st_size;

    // map BIOS at the top of memory
    region.guest_start_paddr = (uint32_t)-bios_size;
    region.memory_size = bios_size;
    char *bios_ptr = (char *)(intptr_t)ioctl(rvm_fd, RVM_GUEST_ADD_MEMORY_REGION, &region);
    printf("bios_ptr = %p\n", bios_ptr);

    read(fd, bios_ptr, bios_size);
    close(fd);

    // map the last 128KB of the BIOS in ISA space */
    int isa_bios_size = bios_size < (128 << 10) ? bios_size : (128 << 10);
    memcpy(ram_ptr + 0x100000 - isa_bios_size, bios_ptr + bios_size - isa_bios_size, isa_bios_size);

    return 0;
}

void init_device(int rvm_fd, int vmid) {
    io_port_init(&IO_PORT, rvm_fd, vmid);
}

int main(int argc, char *argv[]) {
    int fd = open("/dev/rvm", O_RDWR);
    printf("rvm fd = %d\n", fd);

    int vmid = ioctl(fd, RVM_GUEST_CREATE);
    printf("vmid = %d\n", vmid);

    const char *bios_file = "bios.bin";
    if (argc > 1)
        bios_file = argv[1];
    if (init_memory(fd, vmid, bios_file) < 0)
        return 0;

    init_device(fd, vmid);

    // use the HW default entry point CS:RIP = 0xf000:fff0
    struct rvm_vcpu_create_args vcpu_args = {vmid, 0};
    int vcpu_id = ioctl(fd, RVM_VCPU_CREATE, &vcpu_args);

    printf("vcpu_id = %d\n", vcpu_id);

    for (;;) {
        int ret = ioctl(fd, RVM_VCPU_RESUME, vcpu_id);
        printf("RVM_VCPU_RESUME returns %d\n", ret);
        break;
    }

    close(fd);
    return 0;
}
