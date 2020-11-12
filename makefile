floopy=/home/regan/code/os_project/a.img
bochsrc=~/code/os/bochsrc
binhome=/home/regan/code/os_project/bin/
bootinclude=boot/include/
asminclude=include/
vpath %.asm boot:kernel:lib

boot.bin: boot.asm
	nasm $^ -I $(bootinclude) -o $(binhome)$@
loader.bin: loader.asm
	nasm $^ -I $(bootinclude) -o $(binhome)$@

	 

boot:boot.bin loader.bin
	dd if=$(binhome)boot.bin of=$(floopy) count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp $(binhome)loader.bin /mnt/floppy/
	sudo umount /mnt/floppy

kernel.bin:kernel.asm
	nasm $^ -I $(asminclude) -o $(binhome)$@
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp $(binhome)$@ /mnt/floppy/
	sudo umount /mnt/floppy


kernel:kernel.bin
	

.PHONY:boot,debug

debug:
	bochs -f $(bochsrc)
