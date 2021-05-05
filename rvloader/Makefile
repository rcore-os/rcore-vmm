CC = riscv64-linux-musl-gcc
LD = riscv64-linux-musl-ld
CFLAGS = -Wall -mcmodel=medany -g -O0 -ffreestanding
OBJCOPY = riscv64-linux-musl-objcopy
all: rvloader.img
rvloader.img: rvloader.elf
	$(OBJCOPY) rvloader.elf -O binary rvloader.img
rvloader.elf: rvloader.o
	$(LD) -T linker.ld -o rvloader.elf rvloader.o
rvloader.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f rvloader.o rvloader.img rvloader.elf
run: all
	qemu-system-riscv64 -M virt -kernel rvloader.elf -bios ./fw_jump.bin -serial mon:stdio -display none -m 256M
runraw: all
	qemu-system-riscv64 -M virt -device loader,file=rvloader.img,addr=0x80200000 -bios ./fw_jump.bin -serial mon:stdio -display none -m 256M
debug: all
	qemu-system-riscv64 -M virt -kernel rvloader.elf -bios ./fw_jump.bin -serial mon:stdio -display none -m 256M -s -S
debugraw: all
	qemu-system-riscv64 -M virt -device loader,file=rvloader.img,addr=0x80200000 -bios ./fw_jump.bin -serial mon:stdio -display none -m 256M -s -S
