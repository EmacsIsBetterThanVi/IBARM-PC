CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Wall -O2 -fpic -ffreestanding -mcpu=cortex-a7 -nostartfiles

all: kernel7.img

boot.o: boot.s
	arm-none-eabi-gcc $(GCCFLAGS) -DPI_MOD=2 -c boot.s -o boot.o

%.o: %.c
	arm-none-eabi-gcc $(GCCFLAGS) -c $< -o $@

kernel7.img: boot.o $(OFILES)
	arm-none-eabi-ld -nostdlib boot.o $(OFILES) -T linker.ld -o kernel7.elf
	arm-none-eabi-objcopy -O binary kernel7.elf kernel7.img

run:
	qemu-system-arm -machine raspi2b -kernel kernel7.img

clean:
	rm *.elf *.o kernel7.img >/dev/null 2>/dev/null
