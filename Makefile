UNAME := $(shell uname)

ifeq ($(UNAME),Linux)
	CC=gcc -elf_i386
	AS=as --32
	LD=ld -m elf_i386
else
	CC=i386-elf-gcc
	AS=i386-elf-as
	LD=i386-elf-ld
endif

GFLAGS=
CCFLAGS=-m32 -std=c11 -g -O2 -Wall -Wextra -Wpedantic -Wstrict-aliasing
CCFLAGS+=-Wno-pointer-arith -Wno-unused-parameter
CCFLAGS+=-nostdlib -ffreestanding -fno-pie -fno-stack-protector
CCFLAGS+=-fno-builtin-function -fno-builtin -Isrc/include
ASFLAGS=
LDFLAGS=

BOOTSECT_SRCS=\
	src/stage0.S

BOOTSECT_OBJS=$(patsubst src/%.S,build/%.o,$(BOOTSECT_SRCS))

KERNEL_C_SRCS=$(shell find . -name "*.c")
KERNEL_S_SRCS=$(filter-out $(BOOTSECT_SRCS), $(wildcard src/*.S))

KERNEL_OBJS=$(patsubst ./src/%.c,./build/%.o,$(KERNEL_C_SRCS)) $(patsubst src/%.S,build/%.o,$(KERNEL_S_SRCS)) build/image.o

BOOTSECT=bootsect.bin
KERNEL=kernel.bin
IMG=boot.img

default: qemu-mac
all: dirs bootsect kernel

clean:
	find . -name \*.o -type f -delete
	find . -name \*.img -type f -delete
	find . -name \*.elf -type f -delete
	find . -name \*.bin -type f -delete
	@echo $(KERNEL_H_SRCS)
	@echo
	@echo $(KERNEL_OBJS)

../out.bin :
	cd ..;python3 image.py 10

build/image.o : ../out.bin
	cp ../out.bin ./src/image.bin
	objcopy -I binary -O elf32-i386 -B i386 src/image.bin build/image.o

build/%.o : src/%.c
	mkdir -p $(dir $@)
	$(CC) -o $@ -x c -c $< $(GFLAGS) $(CCFLAGS)

build/%.o : src/%.S
	mkdir -p $(dir $@)
	$(AS) -o $@ -c $< $(GFLAGS) $(ASFLAGS)

dirs:
	mkdir -p bin

bootsect: $(BOOTSECT_OBJS)
	$(LD) -o ./bin/$(BOOTSECT) $^ -Ttext 0x7C00 --oformat=binary

kernel: $(KERNEL_OBJS)
	$(LD) -o ./bin/kernel.elf $^ $(LDFLAGS) -Tsrc/link.ld --oformat=elf32-i386
	$(LD) -o ./bin/$(KERNEL) $^ $(LDFLAGS) -Tsrc/link.ld --oformat=binary
	objcopy --only-keep-debug -I elf32-i386 ./bin/kernel.elf bin/kernel.sym

img: dirs bootsect kernel
	rm -f $(IMG)
	dd if=./bin/$(BOOTSECT) of=$(IMG) conv=notrunc bs=512 seek=0 count=1
	dd if=./bin/$(KERNEL) of=$(IMG) conv=notrunc bs=512 seek=1
	dd if=/dev/zero of=$(IMG) bs=1 count=1 seek=130560

iso: img
	mkisofs -pad -b $(img) -R -o boot.iso $(img)

install: img
	sudo dd if=boot.img of=/dev/sda status=progress

qemu-mac: img
	qemu-system-i386 -drive format=raw,file=$(IMG) -vga std -monitor stdio #-S --gdb tcp::6000
qemu-pulse: img
	qemu-system-i386 -drive format=raw,file=$(IMG) -d cpu_reset -monitor stdio -device sb16 -audiodev pulseaudio,id=pulseaudio,out.frequency=48000,out.channels=2,out.format=s32

qemu-sdl: img
	qemu-system-i386 -display sdl -drive format=raw,file=$(IMG) -d cpu_reset -monitor stdio -audiodev sdl,id=sdl,out.frequency=48000,out.channels=2,out.format=s32 -device sb16,audiodev=sdl

qemu-no-audio: img
	qemu-system-i386 -drive format=raw,file=$(IMG) -d cpu_reset -monitor stdio

