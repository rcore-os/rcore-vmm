#ifndef __BIOS_ASM_H__
#define __BIOS_ASM_H__

#define OUTB(port, data)                                             \
    mov $port, %edx;                                                 \
    mov $data, %al;                                                  \
    outb %al, %dx;                                                  \

#endif /* !__BIOS_ASM_H__ */

