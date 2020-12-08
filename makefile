floopy=/home/regan/code/os_project/a.img
bochsrc=~/code/os/bochsrc
binhome=/home/regan/code/os_project/bin/
bootinclude=boot/include/
asminclude=include/
CFLAGS=-m32 -c
kernel_obj= kernel.o   \
cpage.o page.o \
cmemory.o memory.o util.o cutil.o\
cprotect.o protect.o\
cinterrupt.o interrupt.o\
harddisk.o buffer.o \
System.o  ckeyboard.o  ckernel.o cprocess.o filesys.o time.o \
error.o osui.o tty.o
VPATH:=bin kernel/tty kernel/C kernel/ASM kernel/keybord kernel/GUI kernel/err include lib/C lib/ASM kernel/hd boot
%.o:%.asm
	nasm -f elf $^ -I $(asminclude) -o $(binhome)$@

%.o: %.c
	gcc  -w -I $(asminclude) -m32 -c -fno-stack-protector $^  -o $(binhome)$@

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
	ld -m elf_i386 -Ttext 0x30400 -s -o $(binhome)$@  $^ 


kernel:kernel.bin
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp $(binhome)$@.bin /mnt/floppy/
	sudo umount /mnt/floppy



.PHONY:boot,debug

debug:
	bochs -f $(bochsrc)

clean:
	rm bin/*.o
