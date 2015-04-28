PREFIX = /home/stef/tasks/pitchfork/gcc-arm-none-eabi-4_7-2013q3/bin/arm-none-eabi
CC=$(PREFIX)-gcc
LD=$(PREFIX)-ld
OC=$(PREFIX)-objcopy
OD=$(PREFIX)-objdump
AS=$(PREFIX)-as

INCLUDES = -Iinc
CFLAGS = -mno-unaligned-access -g -Wall -Werror -Os \
	-mfix-cortex-m3-ldrd -msoft-float -mthumb -Wno-strict-aliasing \
	-fomit-frame-pointer $(INCLUDES) -mcpu=cortex-m3 \

LDFLAGS = -mthumb -mcpu=cortex-m3 -fno-common -Tmemmap -nostartfiles -Wl,--gc-sections -Wl,-z,relro

objs = tx.o delay.o lib/stm32f10x_gpio.o lib/stm32f10x_spi.o lib/stm32f10x_rcc.o

all : tx.bin

tx.bin : memmap $(objs)
	$(CC) $(LDFLAGS) -o tx.elf $(objs)
	$(OC) tx.elf tx.bin -O binary

clean:
	rm -f *.bin
	rm -f $(objs)
	rm -f *.elf

.PHONY: clean
