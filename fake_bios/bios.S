# fake bios

#include "asm.h"

.set secno, 0

# start address should be 0:0x9000, in real mode
.globl start
start:
.code16                                             # Assemble for 16-bit mode
    cli                                             # Disable interrupts
    cld                                             # String operations increment

    # Set up the important data segment registers (DS, ES, SS).
    xorw %ax, %ax                                   # Segment number zero
    movw %ax, %ds                                   # -> Data Segment
    movw %ax, %es                                   # -> Extra Segment
    movw %ax, %ss                                   # -> Stack Segment

    # read a sector
    OUTB(0x1F2, 1)                         # count = 1
    OUTB(0x1F3, secno & 0xFF);
    OUTB(0x1F4, (secno >> 8) & 0xFF);
    OUTB(0x1F5, (secno >> 16) & 0xFF);
    OUTB(0x1F6, ((secno >> 24) & 0xF) | 0xE0);
    OUTB(0x1F7, 0x20);                      // cmd 0x20 - read sectors

    # insl(0x1F0, 0x7c00, SECTSIZE / 4);
    mov $0x7c00, %edi
    mov $128, %ecx
    mov $0x1f0, %edx
    cld    
    repnz insl

    call setup_ivt
    jmp 0x7c00

    movb $0x00, %al # should not reach here
    outb %al, $0x02
