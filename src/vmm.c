#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dev/bios.h>
#include <dev/ide.h>
#include <dev/io_port.h>
#include <dev/lpt.h>
#include <dev/ps2.h>
#include <dev/serial.h>
#include <dev/vga.h>
#include <mem_set.h>
#include <rvm.h>

#define PTE_P  0x001 // Present
#define PTE_W  0x002 // Writeable
#define PTE_U  0x004 // User
#define PTE_HG 0x080 // Huge page

const uint32_t GUEST_RAM_SZ = 16 * 1024 * 1024; // 16 MiB
const uint32_t PAGE_SIZE = 4096;

struct virt_device IO_PORT;
struct virt_device SERIAL;
struct virt_device IDE;
struct virt_device LPT;
struct virt_device VGA;
struct virt_device PS2;
struct virt_device BIOS;

void test_hypercall() {
    for (int i = 0;; i++) {
        asm volatile("vmcall" : : "a"(i), "b"(2), "c"(3), "d"(3), "S"(3) : "memory");
    }
}

// Warning: currently RVM does not support SeaBIOS.
int init_memory_seabios(int rvm_fd, int vmid, const char* bios_file) {
    int fd = open(bios_file, O_RDONLY);
    if (fd < 0) {
        printf("fail to open BIOS image '%s': %d\n", bios_file, fd);
        return fd;
    }

    // RAM
    struct rvm_guest_add_memory_region_args region = {vmid, 0, GUEST_RAM_SZ};
    char* ram_ptr = (char*)(intptr_t)ioctl(rvm_fd, RVM_GUEST_ADD_MEMORY_REGION, &region);
    if (ram_ptr < (char*)0) {
        printf("fail to add guest memory region: %d\n", (int)(intptr_t)ram_ptr);
        return (intptr_t)ram_ptr;
    }

    struct stat statbuf;
    stat(bios_file, &statbuf);
    int bios_size = statbuf.st_size;

    // map BIOS at the top of memory
    region.guest_start_paddr = (uint32_t)-bios_size;
    region.memory_size = bios_size;
    char* bios_ptr = (char*)(intptr_t)ioctl(rvm_fd, RVM_GUEST_ADD_MEMORY_REGION, &region);
    printf("bios_ptr = %p\n", bios_ptr);

    // Write BIOS image to guest physical memory
    read(fd, bios_ptr, bios_size);
    close(fd);

    // map the last 128KB of the BIOS in ISA space */
    int isa_bios_size = bios_size < (128 << 10) ? bios_size : (128 << 10);
    memcpy(ram_ptr + 0x100000 - isa_bios_size, bios_ptr + bios_size - isa_bios_size, isa_bios_size);

    // use the HW default entry point CS:RIP = 0xf000:fff0
    return 0;
}

int init_memory_ucore_bios(int rvm_fd, int vmid, const char* ucore_bios_file, struct vm_mem_set* mem_set) {
    int fd = open(ucore_bios_file, O_RDONLY);
    if (fd < 0) {
        printf("fail to open BIOS image '%s': %d\n", ucore_bios_file, fd);
        return fd;
    }

    // RAM
    struct rvm_guest_add_memory_region_args region = {vmid, 0, GUEST_RAM_SZ};
    char* ram_ptr = (char*)(intptr_t)ioctl(rvm_fd, RVM_GUEST_ADD_MEMORY_REGION, &region);
    if (ram_ptr < (char*)0) {
        printf("fail to add guest memory region: %d\n", (int)(intptr_t)ram_ptr);
        return (intptr_t)ram_ptr;
    }

    // Setup the guest page table.
    uint64_t* pt0 = (uint64_t*)ram_ptr;
    uint64_t* pt1 = (uint64_t*)(ram_ptr + PAGE_SIZE);
    pt0[0] = PAGE_SIZE | PTE_P | PTE_W | PTE_U;
    pt1[0] = 0 | PTE_P | PTE_W | PTE_U | PTE_HG;

    mem_set_init(mem_set);
    mem_set_push(mem_set, 0, GUEST_RAM_SZ, (uint8_t*)ram_ptr);

    struct stat statbuf;
    stat(ucore_bios_file, &statbuf);
    int bios_size = statbuf.st_size;

    const int UCORE_BIOS_ENTRY = 0x9000; // 36KiB
    printf("UCORE_BIOS_ENTRY = 0x%x\n", UCORE_BIOS_ENTRY);

    // Write BIOS image to guest physical memory
    read(fd, ram_ptr + UCORE_BIOS_ENTRY, bios_size);
    close(fd);

    return UCORE_BIOS_ENTRY;
}

// Warning: currently RVM does not support to boot the raw ucore image without BIOS.
int init_memory_ucore(int rvm_fd, int vmid, const char* ucore_img) {
    int fd = open(ucore_img, O_RDONLY);
    if (fd < 0) {
        printf("fail to open uCore image '%s': %d\n", ucore_img, fd);
        return fd;
    }

    // RAM
    struct rvm_guest_add_memory_region_args region = {vmid, 0, GUEST_RAM_SZ};
    char* ram_ptr = (char*)(intptr_t)ioctl(rvm_fd, RVM_GUEST_ADD_MEMORY_REGION, &region);
    if (ram_ptr < (char*)0) {
        printf("fail to add guest memory region: %d\n", (int)(intptr_t)ram_ptr);
        return (intptr_t)ram_ptr;
    }

    // Write uCore image to guest physical memory
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
    lpt_init(&LPT, rvm_fd, vmid);
    vga_init(&VGA, rvm_fd, vmid);
    ps2_init(&PS2, rvm_fd, vmid);
    bios_init(&BIOS, rvm_fd, vmid);
}

int handle_io(int vcpu_id, struct rvm_exit_io_packet* packet, uint64_t key, struct vm_mem_set* mem_set) {
    if (0x1f0 <= packet->port && packet->port < 0x1f8) {
        // IDE0
    } else if (0x170 <= packet->port && packet->port < 0x170 + 8) {
        // IDE1
    } else if (0x3f8 <= packet->port && packet->port < 0x3ff) {
        // serial
    } else if (0x3D4 <= packet->port && packet->port < 0x3D4 + 8) {
        // VGA
    } else if (0x378 <= packet->port && packet->port < 0x378 + 8) {
        // LPT
    } else if (0x60 <= packet->port && packet->port < 0x60 + 8) {
        // PS2
    } else {
        if (packet->is_input)
            printf("IN %x\n", packet->port);
        else
            printf("OUT %x\n", packet->port);
    }

    struct virt_device* dev = (struct virt_device*)key;

    if (packet->is_repeat) {
        struct rvm_vcpu_state state;
        struct rvm_vcpu_state_args args;
        args.vcpu_id = vcpu_id;
        args.kind = RVM_VCPU_STATE;
        args.vcpu_state_ptr = &state;
        args.buf_size = sizeof(state);
        int ret = ioctl(dev->rvm_fd, RVM_VCPU_READ_STATE, &args);
        if (ret != 0) return ret;

        uint64_t count = state.rcx;
        uint64_t addr = packet->is_input ? state.rdi : state.rsi;

        // FIXME: get right guest_paddr
        uint64_t guest_paddr = (addr >= 0xC0000000) ? (addr - 0xC0000000) : addr; // only for ucore
        // printf("repeat read/write at 0x%x\n", guest_paddr);
        uint8_t* ram_ptr = mem_set_fetch(mem_set, guest_paddr);
        if (ram_ptr == (uint8_t*)(-1)) return -1;

        struct rvm_io_value value;
        value.access_size = packet->access_size;
        if (packet->is_input) {
            for (int i = 0; i < count; i++) {
                int ret = dev->ops->read(dev, packet->port, &value);
                if (ret != 0) return ret;
                for (int k = 0; k < packet->access_size; k++) {
                    *ram_ptr = value.buf[k];
                    ram_ptr++;
                }
            }
            state.rdi += count;
        } else {
            for (int i = 0; i < count; i++) {
                value.u32 = 0;
                for (int k = 0; k < packet->access_size; k++) {
                    value.buf[k] = *ram_ptr;
                    ram_ptr++;
                }
                int ret = dev->ops->write(dev, packet->port, &value);
                if (ret != 0) return ret;
            }
            state.rsi += count;
        }

        state.rcx = 0;
        return ioctl(dev->rvm_fd, RVM_VCPU_WRITE_STATE, &args);
    } else {
        struct rvm_io_value value;
        value.access_size = packet->access_size;
        if (packet->is_input) {
            int ret = dev->ops->read(dev, packet->port, &value);
            if (ret != 0) return ret;

            struct rvm_vcpu_state_args args;
            args.vcpu_id = vcpu_id;
            args.kind = RVM_VCPU_IO;
            args.vcpu_io_ptr = (struct rvm_vcpu_io*)&value;
            args.buf_size = sizeof(value);
            return ioctl(dev->rvm_fd, RVM_VCPU_WRITE_STATE, &args);
        } else {
            value.u32 = packet->u32;
            return dev->ops->write(dev, packet->port, &value);
        }
    }
}

int handle_mmio(int vcpu_id, struct rvm_exit_mmio_packet* packet, uint64_t key) {
    printf("Guest MMIO: addr(0x%lx) [Not supported!]\n", packet->addr);
    return 1;
}

int handle_exit(int vcpu_id, struct rvm_exit_packet* packet, struct vm_mem_set* mem_set) {
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

int main(int argc, char* argv[]) {
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

    int fd = open("/dev/rvm", O_RDWR);
    printf("rvm fd = %d\n", fd);
    if (fd < 0) {
        printf("failed to open /dev/rvm: %d\n", fd);
        return 1;
    }

    int vmid = ioctl(fd, RVM_GUEST_CREATE);
    printf("vmid = %d\n", vmid);
    if (vmid < 0) {
        printf("failed to create guest: %d\n", vmid);
        return 1;
    }

    // if (argc > 1)
    //     img = argv[1];
    // int entry = init_memory_ucore(fd, vmid, img);
    // if (entry < 0)
    //     return 0;

    const char* bios_img = "ucore_bios.bin";
    const char* ucore_img = "ucore.img";
    const char* sfs_img = "sfs.img";
    struct vm_mem_set mem_set;
    int entry = init_memory_ucore_bios(fd, vmid, bios_img, &mem_set);
    if (entry < 0) {
        printf("failed to init memory of Fake BIOS: %d\n", entry);
        return 1;
    }

    init_device(fd, vmid);
    ide_add_file_img(&IDE, ucore_img);          // os image
    ide_add_empty_img(&IDE, 1024 * 1024 * 128); // swap image
    ide_add_file_img(&IDE, sfs_img);            // sfs image

    struct rvm_vcpu_create_args vcpu_args = {vmid, entry};
    int vcpu_id = ioctl(fd, RVM_VCPU_CREATE, &vcpu_args);
    printf("vcpu_id = %d\n", vcpu_id);
    if (vcpu_id < 0) {
        printf("failed to create vcpu: %d\n", vcpu_id);
        return 1;
    }

    for (;;) {
        struct rvm_vcpu_resmue_args args = {vcpu_id};
        int ret = ioctl(fd, RVM_VCPU_RESUME, &args);
        if (ret < 0) {
            printf("failed to resume vcpu: %d\n", ret);
            break;
        }
        ret = handle_exit(vcpu_id, &args.packet, &mem_set);
        if (ret < 0) {
            printf("failed to handle VM exit: kind = %d", args.packet.kind);
            break;
        }
    }

    close(fd);
    return 0;
}
