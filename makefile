floopy=/home/regan/code/os_project/a.img
bochsrc=~/code/os/bochsrc
binhome=/home/regan/code/os_project/bin/
bootinclude=boot/include/
asminclude=include/
CFLAGS		= -m32 -c

vpath %.asm boot:kernel:lib
vpath %.o bin
vpath %.c lib:kernel

boot.bin: boot.asm
	nasm $^ -I $(bootinclude) -o $(binhome)$@
loader.bin: loader.asm
	nasm $^ -I $(bootinclude) -o $(binhome)$@

boot:boot.bin loader.bin
	dd if=$(binhome)boot.bin of=$(floopy) count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp $(binhome)loader.bin /mnt/floppy/
	sudo umount /mnt/floppy

kernel.o : kernel.asm
	nasm -f elf $^ -I $(asminclude) -o $(binhome)$@
test.o: memory.c
	gcc -m32 -c $^ -o $(binhome)test.o

kernel.bin:kernel.o test.o
	ld -m elf_i386 -Ttext 0x35000 -s -o $(binhome)$@  $^ 
kernel:kernel.bin
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp $(binhome)$@.bin /mnt/floppy/
	sudo umount /mnt/floppy

.PHONY:boot,debug

debug:
	bochs -f $(bochsrc)
