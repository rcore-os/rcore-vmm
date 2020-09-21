ARCH ?= x86_64
MODE ?= release

out_dir := build/$(ARCH)

cmake_build_args := -DARCH=$(ARCH)
ifeq ($(MODE), release)
cmake_build_args += -DCMAKE_BUILD_TYPE=Release
else ifeq ($(MODE), debug)
cmake_build_args += -DCMAKE_BUILD_TYPE=Debug
endif

all: build

build: bios vmm

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

clean:
	@rm -rf src/build fake_bios/build
	@rm -rf build

.PHONY: build clean bios vmm
