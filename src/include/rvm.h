#ifndef RVM_H
#define RVM_H

#include <stdbool.h>
#include <stdint.h>

#define PTE_W  0x002 // Writeable
#define PTE_U  0x004 // User
#define PTE_HG 0x080 // Huge page

#define GUEST_RAM_SZ (16 * 1024 * 1024) // 16 MiB
#define PAGE_SIZE 4096


#define RVM_IO                      0xAE00
#define RVM_GUEST_CREATE            (RVM_IO + 0x01)
#define RVM_GUEST_ADD_MEMORY_REGION (RVM_IO + 0x02)
#define RVM_GUEST_SET_TRAP          (RVM_IO + 0x03)
#define RVM_VCPU_CREATE             (RVM_IO + 0x11)
#define RVM_VCPU_RESUME             (RVM_IO + 0x12)
#define RVM_VCPU_READ_STATE         (RVM_IO + 0x13)
#define RVM_VCPU_WRITE_STATE        (RVM_IO + 0x14)
#define RVM_VCPU_INTERRUPT          (RVM_IO + 0x15)

#if defined(__amd64)
    #include "arch/amd64/rvmstruct.h"
#elif defined(__riscv)
    #if __riscv_xlen == 64
        #include "arch/riscv64/rvmstruct.h"
    #else
        #error "RISCV32 unsupported!"
    #endif
#else
    #error "Unsupported architecture!"
#endif

struct rvm_guest_add_memory_region_args {
    uint16_t vmid;
    uint64_t guest_phys_addr;
    uint64_t memory_size;
    void* userspace_addr;
};
struct rvm_guest_set_trap_args {
    uint16_t vmid;
    enum rvm_trap_kind kind;
    uint64_t addr;
    uint64_t size;
    uint64_t key;
};

struct rvm_vcpu_create_args {
    uint16_t vmid;
    uint64_t entry;
};

struct rvm_vcpu_resume_args {
    uint16_t vcpu_id;
    struct rvm_exit_packet packet;
};

struct rvm_vcpu_state_args {
    uint16_t vcpu_id;
    enum rvm_vcpu_read_write_kind kind;
    union {
        struct rvm_vcpu_state* vcpu_state_ptr;
        struct rvm_vcpu_io* vcpu_io_ptr;
    };
    uint64_t buf_size;
};

struct rvm_vcpu_interrupt_args {
    uint16_t vcpu_id;
    uint32_t vector;
};

#endif // RVM_H
