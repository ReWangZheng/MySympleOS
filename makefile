floopy=/home/regan/code/os/a.img
bochsrc=~/code/os/bochsrc
boot.bin: boot.asm
	nasm $^ -o $@
.PHONY:boot,debug

boot:boot.bin
	dd if=boot.bin of=$(floopy) count=1 conv=notrunc
debug:
	bochs -f $(bochsrc)
