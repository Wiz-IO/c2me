CC = gcc

CFLAGS  = -Wall -Wextra -O2 -std=c11 -I./
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-discarded-qualifiers
CFLAGS += -Wno-type-limits

TARGET = firmware.exe

SRCS   = main.c
SRCS  += PIC.c
SRCS  += RV.c
SRCS  += RV_CORE.c
SRCS  += API.c
SRCS  += API_LIBC.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).map $(TARGET).exe *.d

xxx: clean all

.PHONY: all clean xxx