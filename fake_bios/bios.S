# fake bios

#include "asm.h"

.set secno, 0

# start address should be 0:0x9000, in IA-32e mode
.globl start
.code64                                             # Assemble for 64-bit mode
start:
    cli
    cld

clear_PG_PE:
    mov %cr0, %rax
    and $~(CR0_PG|CR0_PE), %eax
    mov %cr4, %rbx
    and $~CR4_PAE, %ebx

    mov $IA32_EFER_MSR, %ecx                        # Set the C-register to 0xC0000080, which is the EFER MSR.

    mov %rax, %cr0                                  # Clear PG and PE in CR0
    jmp clear_PAE_LME

.code16                                             # Assemble for 16-bit mode
clear_PAE_LME:
    mov $0, %edx
    mov %edx, %cr3                                  # Clear CR3 to flush TLB
    mov %ebx, %cr4                                  # Clear PAE in CR4

    rdmsr                                           # Read from the model-specific register.
    andw $~IA32_EFER_LME, %ax                       # Set the LM-bit which is the 9th bit (bit 8).
    wrmsr                                           # Write to the model-specific register.##

    jmp start16

.code16
start16:
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
