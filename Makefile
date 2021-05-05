ARCH ?= x86_64
MODE ?= release
UCORE_LAB ?= 8
PREFIX ?= $(ARCH)-linux-musl-

out_dir := build/$(ARCH)
ucore_dir := ucore/labcodes_answer/lab$(UCORE_LAB)_result

export GCCPREFIX = $(PREFIX)

cmake_build_args := -DARCH=$(ARCH) -DGCCPREFIX=$(PREFIX)
ifeq ($(MODE), release)
cmake_build_args += -DCMAKE_BUILD_TYPE=Release
else ifeq ($(MODE), debug)
cmake_build_args += -DCMAKE_BUILD_TYPE=Debug
endif

all: build-amd64

build-riscv64: vmm rvloader

rvloader:
	@echo Building RISC-V light loader.
	@cd rvloader && make
	@cp rvloader/rvloader.img $(out_dir)
build-x86_64: build-amd64
build-amd64: bios vmm ucore

bios:
	@echo Building uCore BIOS
	@mkdir -p ucore_bios/build
	@cd ucore_bios/build && cmake $(cmake_build_args) .. && make -j
	@mkdir -p $(out_dir)
	@cp ucore_bios/build/ucore_bios.bin $(out_dir)

vmm:
	@echo Building rCore VMM app
	@mkdir -p src/build
	@cd src/build && cmake $(cmake_build_args) .. && make -j
	@mkdir -p $(out_dir)
	@cp src/build/vmm $(out_dir)

ucore: | ucore/*
	@echo Building uCore OS
	# Makefile in labcodes_answer directory is out of date ...
	@cp ucore/labcodes/lab$(UCORE_LAB)/Makefile $(ucore_dir)
	@cd $(ucore_dir) && make
	@cp $(ucore_dir)/bin/ucore.img $(out_dir)
ifeq ($(UCORE_LAB), 8)
	@cp $(ucore_dir)/bin/sfs.img $(out_dir)
endif

clean:
	@rm -rf src/build ucore_bios/build
	@if [ -d $(ucore_dir) ]; then cd $(ucore_dir) && make clean; fi
	@rm -rf build

.PHONY: build clean bios vmm ucore rvloader build-amd64 build-x86_64
