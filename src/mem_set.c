#include "mem_set.h"

void mem_set_init(struct vm_mem_set* mem_set)
{
    mem_set->cnt = 0;
}

void mem_set_push(struct vm_mem_set* mem_set, uint64_t guest_start_paddr, uint64_t memory_size, uint8_t* ram_ptr)
{
    mem_set->mems[mem_set->cnt].guest_start_paddr = guest_start_paddr;
    mem_set->mems[mem_set->cnt].memory_size = memory_size;
    mem_set->mems[mem_set->cnt].ram_ptr = ram_ptr;
    mem_set->cnt ++;
}

uint8_t* mem_set_fetch(struct vm_mem_set* mem_set, uint64_t guest_paddr)
{
    for (int i = 0; i < mem_set->cnt; i ++) {
        if (mem_set->mems[i].guest_start_paddr <= guest_paddr && guest_paddr < mem_set->mems[i].guest_start_paddr + mem_set->mems[i].memory_size) {
            return mem_set->mems[i].ram_ptr + (guest_paddr - mem_set->mems[i].guest_start_paddr);
        }
    }
    return (uint8_t*)(-1);
}
