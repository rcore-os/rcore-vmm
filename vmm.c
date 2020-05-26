#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dev/ide.h>
#include <dev/io_port.h>
#include <dev/serial.h>
#include <rvm.h>

const uint32_t GUEST_RAM_SZ = 16 * 1024 * 1024; // 16 MiB

struct virt_device IO_PORT;
struct virt_device SERIAL;
struct virt_device IDE;

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

int init_memory_bios(int rvm_fd, int vmid, const char *bios_file) {
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

    // use the HW default entry point CS:RIP = 0xf000:fff0
    return 0;
}

int init_memory_ucore(int rvm_fd, int vmid, const char *ucore_img) {
    // RAM
    struct rvm_guest_add_memory_region_args region = {vmid, 0, GUEST_RAM_SZ};
    char *ram_ptr = (char *)(intptr_t)ioctl(rvm_fd, RVM_GUEST_ADD_MEMORY_REGION, &region);

    int fd = open(ucore_img, O_RDONLY);
    if (fd < 0)
        return -1;

    const int SECT_SIZE = 512;
    const int ENTRY = 0x7c00;
    read(fd, ram_ptr + ENTRY, SECT_SIZE);
    close(fd);

    return ENTRY;
}

void init_device(int rvm_fd, int vmid) {
    io_port_init(&IO_PORT, rvm_fd, vmid);
    serial_init(&SERIAL, rvm_fd, vmid);
    ide_init(&IDE, rvm_fd, vmid);
}

int handle_io(int vcpu_id, struct rvm_exit_io_packet *packet, uint64_t key) {
    if (packet->is_input)
        printf("IN %x\n", packet->port);
    else
        printf("OUT %x < %x\n", packet->port, packet->u32);

    struct virt_device *dev = (struct virt_device *)key;
    struct rvm_io_value value = {};
    value.access_size = packet->access_size;
    if (packet->is_input) {
        int ret = dev->ops->read(dev, packet->port, &value);
        if (ret == 0) {
            struct rvm_vcpu_write_state_args state = {
                vcpu_id,
                value.u32,
            };
            ret = ioctl(dev->rvm_fd, RVM_VCPU_WRITE_STATE, &state);
        }
        return ret;
    } else {
        value.u32 = packet->u32;
        return dev->ops->write(dev, packet->port, &value);
    }
}

int handle_mmio(int vcpu_id, struct rvm_exit_mmio_packet *packet, uint64_t key) {
    printf("Guest MMIO: addr(0x%lx) [Not supported!]\n", packet->addr);
    return 1;
}

int handle_exit(int vcpu_id, struct rvm_exit_packet *packet) {
    // printf("Handle guest exit: kind(%d) key(0x%lx)\n", packet->kind, packet->key);
    switch (packet->kind) {
    case RVM_EXIT_PKT_KIND_GUEST_IO:
        return handle_io(vcpu_id, &packet->io, packet->key);
    case RVM_EXIT_PKT_KIND_GUEST_MMIO:
        return handle_mmio(vcpu_id, &packet->mmio, packet->key);
    default:
        return 1;
    }
}

int main(int argc, char *argv[]) {
    int fd = open("/dev/rvm", O_RDWR);
    printf("rvm fd = %d\n", fd);

    int vmid = ioctl(fd, RVM_GUEST_CREATE);
    printf("vmid = %d\n", vmid);

    const char *img = "ucore.img";
    if (argc > 1)
        img = argv[1];
    int entry = init_memory_ucore(fd, vmid, img);
    if (entry < 0)
        return 0;

    init_device(fd, vmid);

    struct rvm_vcpu_create_args vcpu_args = {vmid, entry};
    int vcpu_id = ioctl(fd, RVM_VCPU_CREATE, &vcpu_args);

    printf("vcpu_id = %d\n", vcpu_id);

    for (;;) {
        struct rvm_vcpu_resmue_args args = {vcpu_id};
        int ret = ioctl(fd, RVM_VCPU_RESUME, &args);
        if (ret < 0 || handle_exit(vcpu_id, &args.packet))
            break;
    }

    close(fd);
    return 0;
}
