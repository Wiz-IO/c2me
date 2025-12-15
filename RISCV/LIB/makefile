PP = C:/Users/Georgi/.platformio/packages/toolchain-riscv/bin/
CC = $(PP)riscv64-unknown-elf-gcc

CC_OPTIONS	=-march=rv32ima -mabi=ilp32 -Wall

all:
	@echo * Creating API PIC Library
	$(CC) $(CC_OPTIONS) -g -Os -c api.S -o api.o -fno-builtin
	$(PP)riscv64-unknown-elf-ar rcs libapi.a api.o

