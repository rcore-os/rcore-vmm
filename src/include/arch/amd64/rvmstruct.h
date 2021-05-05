#pragma once
#ifndef RVM_ARCH_AMD64_RVMSTRUCT_H
#define RVM_ARCH_AMD64_RVMSTRUCT_H
enum rvm_trap_kind {
    RVM_TRAP_KIND_BELL = 0,
    RVM_TRAP_KIND_MEM = 1,
    RVM_TRAP_KIND_IO = 2,
};

enum rvm_exit_packet_kind {
    RVM_EXIT_PKT_KIND_GUEST_BELL = 1,
    RVM_EXIT_PKT_KIND_GUEST_IO = 2,
    RVM_EXIT_PKT_KIND_GUEST_MMIO = 3,
    RVM_EXIT_PKT_KIND_GUEST_VCPU = 4,
};

enum rvm_vcpu_read_write_kind {
    RVM_VCPU_STATE = 0,
    RVM_VCPU_IO = 1,
};

struct rvm_vcpu_state {
    uint64_t rax;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbx;
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    // Contains only the user-controllable lower 32-bits.
    uint64_t rflags;
};

typedef struct rvm_io_value rvm_vcpu_io;

struct rvm_io_value {
    uint8_t access_size;
    union {
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint8_t buf[4];
    };
};

struct rvm_exit_io_packet {
    uint16_t port;
    uint8_t access_size;
    bool is_input;
    bool is_string;
    bool is_repeat;
    union {
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint8_t buf[4];
    };
};

struct rvm_exit_mmio_packet {
    uint64_t addr;
    uint8_t inst_len;
    uint8_t inst_buf[15];
    uint8_t default_operand_size;
};

struct rvm_exit_packet {
    enum rvm_exit_packet_kind kind;
    uint64_t key;
    union {
        struct rvm_exit_io_packet io;
        struct rvm_exit_mmio_packet mmio;
    };
};


#endif