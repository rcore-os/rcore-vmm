ARCH ?= x86_64
MODE ?= release
UCORE_LAB ?= 8

prefix := $(ARCH)-linux-musl-
out_dir := build/$(ARCH)
ucore_dir := ucore/labcodes_answer/lab$(UCORE_LAB)_result

export GCCPREFIX = $(prefix)

cmake_build_args := -DARCH=$(ARCH)
ifeq ($(MODE), release)
cmake_build_args += -DCMAKE_BUILD_TYPE=Release
else ifeq ($(MODE), debug)
cmake_build_args += -DCMAKE_BUILD_TYPE=Debug
endif

all: build

build: bios vmm ucore

bios:
	@echo Building Fake BIOS
	@mkdir -p fake_bios/build
	@cd fake_bios/build && cmake $(cmake_build_args) .. && make -j
	@mkdir -p $(out_dir)
	@cp fake_bios/build/fake_bios.bin $(out_dir)

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
	@rm -rf src/build fake_bios/build
	@if [ -d $(ucore_dir) ]; then cd $(ucore_dir) && make clean; fi
	@rm -rf build

.PHONY: build clean bios vmm ucore
