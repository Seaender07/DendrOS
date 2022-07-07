# Makefile

C_SOURCES = $(wildcard kernel/*.c kernel/*/*.c drivers/*.c drivers/*/*.c)
HEADERS = $(wildcard kernel/*.h kernel/*/*.h drivers/*.h drivers/*/*.h)

OBJ = ${C_SOURCES:.c=.o}


edit: $(wildcard boot/*.asm boot/*/*.asm kernel/*.asm kernel/*.c kernel/*.h)  Makefile
	vim -p $^

kernel/kernel.bin: kernel/kern_entry.o ${OBJ}
	ld -o $@ -Ttext 0x1000 $^ -m elf_i386 -s --oformat binary

os-image: boot/boot_sect.bin kernel/kernel.bin
	cat $^ > $@

start: os-image
	qemu-system-i386 -drive format=raw,file=$< -no-reboot


######################################################################################

%.o : %.c ${HEADERS}
	gcc -m32 -fno-pie -ffreestanding -c $< -o $@

%.o : %.asm
	nasm $< -f elf -o $@

%.bin : %.asm
	nasm $< -I boot/ -f bin -o $@





all: os-image

clean:
	rm -rf *.bin *.o
	rm -rf */*.bin */*.o
