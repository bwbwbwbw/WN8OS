SDKDIR=./sdk
KERNELDIR=./kernel

help:
	@echo "Usage: make [ all | clean | help | pack | run ]" 
	@echo ""
	@echo

all:
	@echo "Building WN8OS kernel..."
	make -C $(KERNELDIR)

	@echo "Packing kernel..."
	-sudo losetup -d /dev/loop1 >/dev/null
	-sudo losetup -d /dev/loop0 >/dev/null
	cd $(SDKDIR) && sudo bash makeimage.sh

run:
	cd $(SDKDIR) && bochs -f bochsrc -q

qemu:
	cd $(SDKDIR) && bash qemu.sh

debug:
	cd $(SDKDIR) && gdb -x gdb_remote

clean:
	make -C $(KERNELDIR) clean