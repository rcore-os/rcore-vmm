asm(".pushsection .bss \n\
stack_region_start: \n\
.skip 4096*4 \n\
stack_region_end: \n\
.popsection\n\
.global _start\n\
_start:\n\
la sp, stack_region_end\n\
call main\n\
halt_device:\n\
j halt_device\n\
");


void console_putchar(int ch){
    asm("li a7, 1; mv a0, %0;ecall; ": :"r"(ch):"a0","a1","a7");
}

void main(){
    const char* str = "Hello, world\n";
    const char* ptr = str;
    while(*ptr!=0){
        console_putchar(*ptr);
        ptr++;
    }
}