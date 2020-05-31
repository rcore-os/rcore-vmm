#ifndef VMM_MEM_SET_H
#define VMM_MEM_SET_H

#include <stdint.h>

#define MAX_MEM_SET 16

struct vm_mem_set {
    int cnt;
    struct {
        uint64_t guest_start_paddr;
        uint64_t memory_size;
        uint8_t* ram_ptr;
    } mems[MAX_MEM_SET];
};

void mem_set_init(struct vm_mem_set* mem_set);
void mem_set_push(struct vm_mem_set* mem_set, uint64_t guest_start_paddr, uint64_t memory_size, uint8_t* ram_ptr);
uint8_t* mem_set_fetch(struct vm_mem_set* mem_set, uint64_t guest_paddr);

#endif // VMM_MEM_SET_H
