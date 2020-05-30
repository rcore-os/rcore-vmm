
ucore的物理内存使用布局：

总大小：16MiB

0x7C00(31K) ~ 512 : bootloader
0x8000(32K) ~ 404 : e820map 物理内存布局，由bios填写
0x10000(64K) ~ 512*8 : ELF header
0x100000(1M) ~ ? : kernel

0x9000(36K) ~ ? : bios

memory: 0x00000000 0x0009fc00 0x00000001
memory: 0x0009fc00 0x00000400 0x00000002
memory: 0x000f0000 0x00010000 0x00000002
memory: 0x00100000 0x07ee0000 0x00000001
memory: 0x07fe0000 0x00020000 0x00000002
memory: 0xfffc0000 0x00040000 0x00000002

memory: 0009fc00, [00000000, 0009fbff], type = 1.
memory: 00000400, [0009fc00, 0009ffff], type = 2.
memory: 00010000, [000f0000, 000fffff], type = 2.
memory: 07ee0000, [00100000, 07fdffff], type = 1.
memory: 00020000, [07fe0000, 07ffffff], type = 2.
memory: 00040000, [fffc0000, ffffffff], type = 2.