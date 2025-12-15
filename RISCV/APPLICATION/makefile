# Toolchain settings
PP = C:/Users/Georgi/.platformio/packages/toolchain-riscv/bin/
CC = $(PP)riscv64-unknown-elf-gcc
OBJCOPY = $(PP)riscv64-unknown-elf-objcopy
OBJDUMP = $(PP)riscv64-unknown-elf-objdump
SIZE = $(PP)riscv64-unknown-elf-size

TARGET = firmware
DUMPFILE = $(TARGET:.elf=.S)

CORE  = -march=rv32ima
CORE += -mabi=ilp32

# Compiler flags
CFLAGS  = $(CORE)
CFLAGS += -Os
CFLAGS += -std=gnu99
CFLAGS += -fomit-frame-pointer
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -fno-asm
CFLAGS += -fno-common
CFLAGS += -fno-builtin
CFLAGS += -fno-strict-aliasing
CFLAGS += -fsingle-precision-constant
CFLAGS += -Wall
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-value

# Linker flags
LDFLAGS  = $(CORE)
LDFLAGS += -Os
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -lnosys
LDFLAGS += -nostartfiles
#LDFLAGS += --specs=nano.specs --specs=nosys.specs
#LDFLAGS += -u _printf_float
LDFLAGS += -L../LIB -lapi
LDFLAGS += -T bin.ld
LDFLAGS += --entry=main

# Source files
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Build rules
all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@
	$(OBJDUMP) -d -M no-aliases $@ > $(DUMPFILE)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).map *.d

.PHONY: all clean