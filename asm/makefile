floopy=/home/regan/code/os/a.img
bochsrc=~/code/os/bochsrc
binhome=/home/regan/code/os_project/bin/
boot.bin: boot.asm
	nasm $^ -o  $(join $(binhome),$@)

.PHONY:boot,debug

boot:boot.bin
	dd if=$(binhome)boot.bin of=$(floopy) count=1 conv=notrunc
debug:
	bochs -f $(bochsrc)
