
SETUP_IVT_TEMPLATE = """
# setup ivt function

.globl setup_ivt
setup_ivt:
.code16                                             # Assemble for 16-bit mode
SETUP_IVT_HERE
    ret

.text
IVT_VECTORS_HERE

int_0x15:
    mov $memmap, %esi
    add %ebx, %esi
    rep; movsb;
    add $20, %ebx
    sub $20, %edi

    cmp $(memmap_end-memmap), %ebx
    jl tmp0
    mov $0, %ebx
tmp0:
    
    iret

.p2align 2
.data
memmap:
MEMMAP_HERE
memmap_end:
"""

with open('setup_ivt.S', 'w') as fd:
    template = SETUP_IVT_TEMPLATE

    IVT_VECTORS = []
    for i in range(256):
        IVT_VECTORS.append(f'.globl vector{i}')
        IVT_VECTORS.append(f'vector{i}:')
        if i == 0x15:
            IVT_VECTORS.append(f'\tjmp int_0x15')
        else:
            IVT_VECTORS.append(f'\tmovw ${i&0xFF}, %ax')
            IVT_VECTORS.append(f'\toutw %ax, $0x01')
    template = template.replace('IVT_VECTORS_HERE', '\n'.join(IVT_VECTORS))

    SETUP_IVT = []
    for i in range(256):
        SETUP_IVT.append(f'\tmovw $vector{i}, %ax')
        SETUP_IVT.append(f'\tmovw %ax, {i*4}')
        SETUP_IVT.append(f'\tmovw $0x0000, {i*4+2}')
    template = template.replace('SETUP_IVT_HERE', '\n'.join(SETUP_IVT))

    memmap = [ # 物理内存布局 (uint64:addr, uint64:size, uint32:type)
        [0x00000000, 0x0009fc00, 0x00000001],
        [0x0009fc00, 0x00000400, 0x00000002],
        [0x000f0000, 0x00010000, 0x00000002],
        [0x00100000, 0x07ee0000, 0x00000001],
        [0x07fe0000, 0x00020000, 0x00000002],
        [0xfffc0000, 0x00040000, 0x00000002]
    ]
    MEMMAP = []
    for mem in memmap:
        MEMMAP.append(f'\t.long {hex(mem[0]&0xFFFFFFFF)}, {hex((mem[0]>>32)&0xFFFFFFFF)}')
        MEMMAP.append(f'\t.long {hex(mem[1]&0xFFFFFFFF)}, {hex((mem[1]>>32)&0xFFFFFFFF)}')
        MEMMAP.append(f'\t.long {hex(mem[2])}')
        MEMMAP.append(f'\t')
    template = template.replace('MEMMAP_HERE', '\n'.join(MEMMAP))

    fd.write(template)
