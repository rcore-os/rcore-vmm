# rcore-vmm

[![CI](https://github.com/rcore-os/rcore-vmm/workflows/CI/badge.svg?branch=master)](https://github.com/rcore-os/rcore-vmm/actions)

VMM (Virtual Machine Monitor) app for [rCore OS](https://github.com/rcore-os/rCore) runs on top of [RVM](https://github.com/rcore-os/RVM).

Can run the unmodified [ucore_os_lab](https://github.com/chyyuu/os_kernel_lab/tree/master) as a guest OS in rCore.

## Build

Install prebuilt musl toolchain from [musl.cc](https://musl.cc/).

Then, build userspace VMM app for rCore:

```bash
$ make [ARCH=x86_64]
```

## How to run in rCore

Clone rCore recursively with [rcore-user](https://github.com/rcore-os/rcore-user), [rcore-vmm](https://github.com/rcore-os/rcore-vmm) and [ucore_os_lab](https://github.com/chyyuu/os_kernel_lab/tree/master):

```bash
$ git clone https://github.com/rcore-os/rCore.git --recursive
```

Build rcore-user with `EN_VMM=y`:

```bash
$ cd rCore/user
$ make sfsimg ARCH=x86_64 EN_VMM=y
```

Build and run rCore with `HYPERVISOR=on`:

```bash
$ cd ../kernel
$ make run mode=release ARCH=x86_64 HYPERVISOR=on
```

Run the `vmm` app in rCore shell:

```
Hello world! from CPU 0!
/ # cd vmm
/vmm # ./vmm
rvm fd = 3
vmid = 1
UCORE_BIOS_ENTRY = 0x9000
[INFO] success to map ucore.img to ide base:disk = 0:0
[INFO] success to add empty disk 134217728 bytes, base:disk = 0:1
[INFO] success to map sfs.img to ide base:disk = 1:0
vcpu_id = 1

(THU.CST) os is loading ...
```

Now uCore is booting and your can get uCore's shell soon.
