#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mem_set.h>
#include <dev/ide.h>
#include <dev/io_port.h>
#include <dev/serial.h>
#include <dev/lpt.h>
#include <dev/vga.h>
#include <dev/ps2.h>
#include <dev/bios.h>
#include <rvm.h>

const uint32_t GUEST_RAM_SZ = 16 * 1024 * 1024; // 16 MiB

struct virt_device IO_PORT;
struct virt_device SERIAL;
struct virt_device IDE;
struct virt_device LPT;
struct virt_device VGA;
struct virt_device PS2;
struct virt_device BIOS;

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

int init_memory_fake_bios(int rvm_fd, int vmid, const char *fake_bios_file, struct vm_mem_set* mem_set) {
    mem_set_init(mem_set);

    // RAM
    struct rvm_guest_add_memory_region_args region = {vmid, 0, GUEST_RAM_SZ};
    char *ram_ptr = (char *)(intptr_t)ioctl(rvm_fd, RVM_GUEST_ADD_MEMORY_REGION, &region);
    mem_set_push(mem_set, 0, GUEST_RAM_SZ, ram_ptr);

    int fd = open(fake_bios_file, O_RDONLY);
    if (fd < 0)
        return -1;

    struct stat statbuf;
    stat(fake_bios_file, &statbuf);
    int bios_size = statbuf.st_size;

    const int FAKE_BIOS_ENTRY = 0x9000; // 36KiB
    printf("FAKE_BIOS_ENTRY = 0x%x\n", FAKE_BIOS_ENTRY);
    
    read(fd, ram_ptr + FAKE_BIOS_ENTRY, bios_size);
    close(fd);

    return FAKE_BIOS_ENTRY;
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

void init_device(int rvm_fd, int vmid, const char* ide_img) {
    io_port_init(&IO_PORT, rvm_fd, vmid);
    serial_init(&SERIAL, rvm_fd, vmid);
    ide_init(&IDE, rvm_fd, vmid, ide_img);
    lpt_init(&LPT, rvm_fd, vmid);
    vga_init(&VGA, rvm_fd, vmid);
    ps2_init(&PS2, rvm_fd, vmid);
    bios_init(&BIOS, rvm_fd, vmid);
}

int handle_io(int vcpu_id, struct rvm_exit_io_packet *packet, uint64_t key, struct vm_mem_set* mem_set) {
    if (0x1f0 <= packet->port && packet->port < 0x1f8) {
        // IDE has too many info
    } else if (0x3f8 <= packet->port && packet->port < 0x3ff) {
        // serial
    } else if (0x3D4 <= packet->port && packet->port < 0x3D4+8) {
        // VGA
    } else if (0x378 <= packet->port && packet->port < 0x378+8) {
        // LPT
    } else if (0x60 <= packet->port && packet->port < 0x60+8) {
        // PS2
    } else {
        if (packet->is_input)
            printf("IN %x\n", packet->port);
        else
            printf("OUT %x\n", packet->port);
    }

    struct virt_device *dev = (struct virt_device *)key;

    struct rvm_vcpu_state_args state_args;
    state_args.vcpu_id = vcpu_id;
    {
        int ret = ioctl(dev->rvm_fd, RVM_VCPU_READ_STATE, &state_args);
        if (ret != 0) return ret;
    }

    if (packet->is_input) {
        if (packet->is_repeat) {
            uint64_t rcx = state_args.guest_state.rcx;
            uint64_t rdi = state_args.guest_state.rdi;

            // FIXME: get right guest_paddr
            uint64_t guest_paddr = (rdi >= 0xC0000000) ? (rdi - 0xC0000000) : rdi; // only for ucore
            printf("repeat read at 0x%x\n", guest_paddr);

            uint8_t* ram_ptr = mem_set_fetch(mem_set, guest_paddr);
            if (ram_ptr == (uint8_t*)(-1)) return -1;

            for (int i = 0; i < rcx; i ++) {
                union rvm_io_value value;
                int ret = dev->ops->read(dev, packet->port, packet->access_size, &value);
                if (ret != 0) return ret;
                for (int k = 0; k < packet->access_size; k ++) {
                    *ram_ptr = value.buf[k];
                    ram_ptr ++;
                }
            }

            state_args.guest_state.rdi += rcx;
            state_args.guest_state.rcx = 0;
            return ioctl(dev->rvm_fd, RVM_VCPU_WRITE_STATE, &state_args);
        } else {
            union rvm_io_value value;
            int ret = dev->ops->read(dev, packet->port, packet->access_size, &value);
            if (ret != 0) return ret;
            state_args.guest_state.rax = value.u32;
            return ioctl(dev->rvm_fd, RVM_VCPU_WRITE_STATE, &state_args);
        }
    } else {
        if (packet->is_repeat) {
            uint64_t rcx = state_args.guest_state.rcx;
            uint64_t rsi = state_args.guest_state.rsi;

            // FIXME: get right guest_paddr
            uint64_t guest_paddr = (rsi >= 0xC0000000) ? (rsi - 0xC0000000) : rsi; // only for ucore
            printf("repeat write at 0x%x\n", guest_paddr);

            uint8_t* ram_ptr = mem_set_fetch(mem_set, guest_paddr);
            if (ram_ptr == (uint8_t*)(-1)) return -1;

            for (int i = 0; i < rcx; i ++) {
                union rvm_io_value value;
                value.u32 = 0;
                for (int k = 0; k < packet->access_size; k ++) {
                    value.buf[k] = *ram_ptr;
                    ram_ptr ++;
                }
                int ret = dev->ops->write(dev, packet->port, packet->access_size, &value);
                if (ret != 0) return ret;
            }

            state_args.guest_state.rsi += rcx;
            state_args.guest_state.rcx = 0;
            return ioctl(dev->rvm_fd, RVM_VCPU_WRITE_STATE, &state_args);
        } else {
            union rvm_io_value value;
            value.u32 = state_args.guest_state.rax;
            int ret = dev->ops->write(dev, packet->port, packet->access_size, &value);
            if (ret != 0) return ret;
            return 0;
        }
    }
}

int handle_mmio(int vcpu_id, struct rvm_exit_mmio_packet *packet, uint64_t key) {
    printf("Guest MMIO: addr(0x%lx) [Not supported!]\n", packet->addr);
    return 1;
}

int handle_exit(int vcpu_id, struct rvm_exit_packet *packet, struct vm_mem_set* mem_set) {
    // printf("Handle guest exit: kind(%d) key(0x%lx)\n", packet->kind, packet->key);
    switch (packet->kind) {
    case RVM_EXIT_PKT_KIND_GUEST_IO:
        return handle_io(vcpu_id, &packet->io, packet->key, mem_set);
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

    // if (argc > 1)
    //     img = argv[1];
    // int entry = init_memory_ucore(fd, vmid, img);
    // if (entry < 0)
    //     return 0;
    
    const char *bios_img = "/app/fake_bios.bin";
    const char *ucore_img = "/app/ucore.img";
    struct vm_mem_set mem_set;
    int entry = init_memory_fake_bios(fd, vmid, bios_img, &mem_set);
    if (entry < 0)
        return 0;

    init_device(fd, vmid, ucore_img);

    struct rvm_vcpu_create_args vcpu_args = {vmid, entry};
    int vcpu_id = ioctl(fd, RVM_VCPU_CREATE, &vcpu_args);

    printf("vcpu_id = %d\n", vcpu_id);

    for (;;) {
        struct rvm_vcpu_resmue_args args = {vcpu_id};
        int ret = ioctl(fd, RVM_VCPU_RESUME, &args);
        if (ret < 0 || handle_exit(vcpu_id, &args.packet, &mem_set))
            break;
    }

    close(fd);
    return 0;
}
