SDKDIR=./sdk
KERNELDIR=./kernel

help:
	@echo "Usage: make [ all | clean | help | pack | run ]" 
	@echo ""
	@echo

all:
	@echo "Building WN8OS kernel..."
	make -C $(KERNELDIR)

pack:
	@echo "Packing kernel..."
	cd $(SDKDIR) && sudo bash makeimage.sh

run:
	cd $(SDKDIR) && bochs -f bochsrc -q

qemu:
	cd $(SDKDIR) && bash qemu.sh

clean:
	make -C $(KERNELDIR) clean