CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
PIMOD = 2
GCCFLAGS = -Wall -Wextra -nostdlib -mgeneral-regs-only -fpic -ffreestanding -mcpu=cortex-a7 -nostartfiles -DPI_MOD=$(PIMOD)
V=@

all: kernel7.img

boot.o: boot.s
	$(V)arm-none-eabi-gcc $(GCCFLAGS) -c boot.s -o boot.o

%.o: %.c
	$(V)arm-none-eabi-gcc $(GCCFLAGS) -c $< -o $@

kernel7.img: boot.o $(OFILES)
	$(V)arm-none-eabi-gcc -T linker.ld -o kernel7.elf -ffreestanding -nostdlib boot.o $(OFILES) -lgcc
	$(V)arm-none-eabi-objcopy kernel7.elf -O binary kernel7.img
debug: GCCFLAGS = -Wall -Wextra -nostdlib -mgeneral-regs-only -fpic -ffreestanding -mcpu=cortex-a7 -nostartfiles -DPI_MOD=$(PIMOD) -DDEBUG=1
debug:	kernel7.img
test: kernel7.img
	$(V)qemu-system-arm -machine raspi2b -kernel kernel7.elf -serial stdio

run:
	$(V)qemu-system-arm -machine raspi2b -kernel kernel7.img

clean:
	$(V)rm -f *.elf *.o kernel7.img 2>/dev/null
