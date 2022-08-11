# Makefile

C_SOURCES = $(wildcard kernel/*.c kernel/*/*.c drivers/*.c drivers/*/*.c)
HEADERS = $(wildcard kernel/*.h kernel/*/*.h drivers/*.h drivers/*/*.h)

OBJ = ${C_SOURCES:.c=.o}


edit: $(wildcard boot/*.asm boot/*/*.asm kernel/*.asm kernel/*.c kernel/*.h drivers/*.c drivers/*.h drivers/*/*.h drivers/*/*.c include/*.h) Makefile
	vim -p $^

kernel/kernel.bin: kernel/kern_entry.o ${OBJ}
	ld -o $@ -Ttext 0x1000 $^ -m elf_i386 -s --oformat binary

os-image: boot/boot_sect.bin kernel/kernel.bin
	cat $^ > $@

os-image-padded: os-image
	dd if=$< of=$@ bs=100M conv=sync

vm-disk.vdi: os-image-padded
	vboxmanage convertfromraw $< $@ --format VDI

start: os-image
	qemu-system-i386 -drive format=raw,file=$< -no-reboot

vmbake: vm-disk.vdi


######################################################################################

%.o : %.c ${HEADERS}
	gcc -m32 -fno-pie -fno-stack-protector -ffreestanding -c $< -o $@

%.o : %.asm
	nasm $< -f elf -o $@

%.bin : %.asm
	nasm $< -I boot/ -f bin -o $@





all: os-image

clean:
	rm -rf *.bin *.o
	rm -rf */*.bin */*.o
	rm -rf */*/*.bin */*/*.o

cleanbin:
	rm -rf os-image os-image-padded *.vdi
