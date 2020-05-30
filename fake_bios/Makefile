V       := @

# try to infer the correct GCCPREFX
ifndef GCCPREFIX
GCCPREFIX := $(shell if i386-elf-objdump -i 2>&1 | grep '^elf32-i386$$' >/dev/null 2>&1; \
	then echo 'i386-elf-'; \
	elif objdump -i 2>&1 | grep 'elf32-i386' >/dev/null 2>&1; \
	then echo ''; \
	else echo "***" 1>&2; \
	echo "*** Error: Couldn't find an i386-elf version of GCC/binutils." 1>&2; \
	echo "*** Is the directory with i386-elf-gcc in your PATH?" 1>&2; \
	echo "*** If your i386-elf toolchain is installed with a command" 1>&2; \
	echo "*** prefix other than 'i386-elf-', set your GCCPREFIX" 1>&2; \
	echo "*** environment variable to that prefix and run 'make' again." 1>&2; \
	echo "*** To turn off this error, run 'gmake GCCPREFIX= ...'." 1>&2; \
	echo "***" 1>&2; exit 1; fi)
endif

# eliminate default suffix rules
.SUFFIXES: .c .S .h

# delete target files if there is an error (or make is interrupted)
.DELETE_ON_ERROR:

# define compiler and flags

HOSTCC		:= gcc
HOSTCFLAGS	:= -g -Wall -O2

GDB		:= $(GCCPREFIX)gdb

CC		:= $(GCCPREFIX)gcc
CFLAGS	:= -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc $(DEFS)
CFLAGS	+= $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)
CTYPE	:= c S

LD      := $(GCCPREFIX)ld
LDFLAGS	:= -m $(shell $(LD) -V | grep elf_i386 2>/dev/null | head -n 1)
LDFLAGS	+= -nostdlib

OBJCOPY := $(GCCPREFIX)objcopy
OBJDUMP := $(GCCPREFIX)objdump

COPY	:= cp
MKDIR   := mkdir -p
MV		:= mv
RM		:= rm -f
AWK		:= awk
SED		:= sed
SH		:= sh
TR		:= tr
TOUCH	:= touch -c

TAR		:= tar
ZIP		:= gzip

OBJDIR	:= obj
BINDIR	:= bin

ALLOBJS	:=
ALLDEPS	:=
TARGETS	:=

all: bin/fake_bios.bin

obj/%.o: %.S
	@echo CC $@
	$(V)$(MKDIR) obj
	$(V)$(CC) $(CFLAGS) -Os -nostdinc $< -c -o $@

obj/%.o: %.c
	@echo CC $@
	$(V)$(MKDIR) obj
	$(V)$(CC) $(CFLAGS) -Os -nostdinc $< -c -o $@

bin/fake_bios.bin: obj/bios.o obj/setup_ivt.o bios.ld
	@echo + LD $@
	$(V)$(MKDIR) bin
	$(V)$(LD) $(LDFLAGS) -N -T bios.ld obj/bios.o obj/setup_ivt.o -o bin/bios.exe
	@$(OBJDUMP) -S bin/bios.exe > bin/bios.asm
	@$(OBJCOPY) -S -O binary bin/bios.exe $@

.PHONY: clean
clean:
	$(V)$(RM) -r obj bin
