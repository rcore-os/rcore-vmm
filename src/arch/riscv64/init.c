#include <stdio.h>
#include <rvm.h>
#include <stdlib.h>
#include <dev/dev.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mem_set.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int handle_ecall(int vcpu_id, struct rvm_exit_ecall_packet* packet, struct vm_mem_set* mem_set){
    (void)vcpu_id;
    (void)mem_set;
    if(packet->eid==0x01){
        putchar((int)packet->args[0]);
        return 0;
    }else{
        printf("Unimplemented SBI call: eid=%d, fid=%d\n", packet->eid, packet->fid);
        return 0;
    }
}
int handle_mmio(int vcpu_id, struct rvm_exit_mmio_packet* packet, uint64_t key, struct vm_mem_set* mem_set) {
    (void)vcpu_id;
    (void)mem_set;
    (void)key;
    printf("Guest MMIO: addr(0x%lx) [Not supported!]\n", packet->addr);
    return 1;
}

int handle_exit(int vcpu_id, struct rvm_exit_packet* packet, struct vm_mem_set* mem_set) {
    // printf("Handle guest exit: kind(%d) key(0x%lx)\n", packet->kind, packet->key);
    switch (packet->kind) {
    case RVM_EXIT_PKT_KIND_GUEST_ECALL:
        return handle_ecall(vcpu_id, &packet->ecall, mem_set);
    case RVM_EXIT_PKT_KIND_GUEST_MMIO:
        return handle_mmio(vcpu_id, &packet->mmio, packet->key, mem_set);
    default:
        return 1;
    }
}

long long int load_image(int rvm_fd, short unsigned int vmid, const char* image_path, struct vm_mem_set* mem_set){
    int image_fd = open(image_path, O_RDONLY);
    if (image_fd<0){
        printf("Failed to load image: %s (%d)\n", image_path, image_fd);
    }
    const int64_t memory_size = 128*1024*1024;
    struct stat statbuf;
    stat(image_path, &statbuf);
    int64_t image_size = statbuf.st_size;
    if(image_size>memory_size){
        printf("Image size exceeds 128MiB.\n");
        return -1;
    }
    struct rvm_guest_add_memory_region_args region = {
        .guest_phys_addr= 0x80200000,
        .memory_size = memory_size,
        .vmid = vmid,
    };
    intptr_t ram_ptr_val = (intptr_t)ioctl(rvm_fd, RVM_GUEST_ADD_MEMORY_REGION, &region);
    if (ram_ptr_val < 0){
        printf("failed to add guest memory region: %d\n", (int)ram_ptr_val);
        return (int)ram_ptr_val;
    }
    char* ram_ptr = (char*)ram_ptr_val;
    if(read(image_fd, ram_ptr, (size_t)image_size)!=(ssize_t)image_size){
        printf("failed to read image.\n");
        return -2;
    }
    close(image_fd);
    mem_set_push(mem_set, 0, memory_size, (unsigned char*)ram_ptr);
    return 0x80200000;
}

int init_vm(int fd, int vmid, int* vcpu_id_p, struct vm_mem_set** mem_set_p){
    const char* simple_img = "rvloader.img";
    *mem_set_p = malloc(sizeof(struct vm_mem_set));
    struct vm_mem_set* mem_set = *mem_set_p;
    mem_set_init(mem_set);
    long long entry = load_image(fd, (unsigned short)vmid, simple_img, mem_set);
    if (entry < 0) {
        printf("failed to init memory: %lld\n", entry);
        return 1;
    }


    struct rvm_vcpu_create_args vcpu_args = {
        .vmid = (unsigned short)vmid, 
        .entry = (unsigned long long) entry
    };
    int vcpu_id = ioctl(fd, RVM_VCPU_CREATE, &vcpu_args);
    printf("vcpu_id = %d\n", vcpu_id);
    if (vcpu_id < 0) {
        printf("failed to create vcpu: %d\n", vcpu_id);
        return 1;
    }
    *vcpu_id_p = vcpu_id;
    return 0;

}