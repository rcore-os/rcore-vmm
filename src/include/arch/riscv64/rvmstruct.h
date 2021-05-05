#pragma once
#ifndef RVM_ARCH_RISCV64_RVMSTRUCT_H
#define RVM_ARCH_RISCV64_RVMSTRUCT_H
#include <stdint.h>
enum rvm_trap_kind {
    RVM_TRAP_KIND_MEM = 1, // there is no other trap but mem trap.
};

enum rvm_exit_packet_kind {
    RVM_EXIT_PKT_KIND_GUEST_ECALL = 1,
    RVM_EXIT_PKT_KIND_GUEST_MMIO = 3,
    RVM_EXIT_PKT_KIND_GUEST_VCPU = 4,
};

enum rvm_vcpu_read_write_kind {
    RVM_VCPU_STATE = 0,
    RVM_VCPU_IO = 1,
};
typedef uint64_t uxlen_t;
struct rvm_vcpu_state {
    uxlen_t sp_kernel;
    uxlen_t ra;
    uxlen_t sp;
    uxlen_t gp;
    uxlen_t tp;
    uxlen_t t0;
    uxlen_t t1;
    uxlen_t t2;
    uxlen_t s0;
    uxlen_t s1;
    uxlen_t a0;
    uxlen_t a1;
    uxlen_t a2;
    uxlen_t a3;
    uxlen_t a4;
    uxlen_t a5;
    uxlen_t a6;
    uxlen_t a7;
    uxlen_t s2;
    uxlen_t s3;
    uxlen_t s4;
    uxlen_t s5;
    uxlen_t s6;
    uxlen_t s7;
    uxlen_t s8;
    uxlen_t s9;
    uxlen_t s10;
    uxlen_t s11;
    uxlen_t t3;
    uxlen_t t4;
    uxlen_t t5;
    uxlen_t t6;
    uxlen_t sstatus;
    uxlen_t sepc;
};
/*
typedef void* rvm_vcpu_io;

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
*/

// riscv sbi.
struct rvm_exit_ecall_packet{
    int eid;
    int fid;
    uint64_t args[4];
};

struct rvm_mmio_value{
    uint8_t access_size;
    uint8_t read;
    uint64_t data;
    uint8_t dstreg;
    uint8_t extension;
    uint32_t insn;
};

struct rvm_exit_mmio_packet {
    uint64_t addr;
    uint8_t access_size;
    uint8_t read;
    uint8_t execute;
    uint64_t data;
    uint8_t dstreg;
    uint8_t extension;
    uint32_t insn;
};

struct rvm_exit_packet {
    enum rvm_exit_packet_kind kind;
    uint64_t key;
    union {
        struct rvm_exit_ecall_packet ecall;
        struct rvm_exit_mmio_packet mmio;
    };
};


#endif