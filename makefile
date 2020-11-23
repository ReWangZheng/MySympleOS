floopy=/home/regan/code/os_project/a.img
bochsrc=~/code/os/bochsrc
binhome=/home/regan/code/os_project/bin/
bootinclude=boot/include/
asminclude=include/
CFLAGS=-m32 -c
kernel_obj=kernel.o util.o System.o cutil.o \
protect.o memory.o cprotect.o \
cinterrupt.o interrupt.o \
page.o cpage.o process.o\
cprocess.o ckernel.o cmemory.o\

VPATH:=bin kernel/C kernel/ASM include lib/C lib/ASM boot
%.o:%.asm
	nasm -f elf $^ -I $(asminclude) -o $(binhome)$@

%.o: %.c
	gcc -I $(asminclude) -m32 -c -fno-stack-protector $^ -o $(binhome)$@

boot.bin: boot.asm
	nasm $^ -I $(bootinclude) -o $(binhome)$@
loader.bin: loader.asm
	nasm $^ -I $(bootinclude) -o $(binhome)$@

boot:boot.bin loader.bin
	dd if=$(binhome)boot.bin of=$(floopy) count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp $(binhome)loader.bin /mnt/floppy/
	sudo umount /mnt/floppy
kernel.bin:$(kernel_obj)
	ld -m elf_i386 -Ttext 0x35000 -s -o $(binhome)$@  $^ 


kernel:kernel.bin
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp $(binhome)$@.bin /mnt/floppy/
	sudo umount /mnt/floppy



.PHONY:boot,debug

debug:
	bochs -f $(bochsrc)

clean:
	rm bin/*.o
