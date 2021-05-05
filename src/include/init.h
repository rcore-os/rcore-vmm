#pragma once
#ifndef RVM_ARCH_AMD64_INIT
#define RVM_ARCH_AMD64_INIT
#include "../../mem_set.h"
#include <rvm.h>
int init_vm(int fd, int vmid, int* vcpu_id_p, struct vm_mem_set** mem_set_p);
int handle_exit(int vcpu_id, struct rvm_exit_packet* packet, struct vm_mem_set* mem_set);
#endif

