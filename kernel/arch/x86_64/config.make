LD=ld
LDFLAG= -nostdinc -T ./arch/$(ARCH)/linker.ld

SC=g++
SCFLAG= $(INCDIR) -g -O2 -w -trigraphs -fno-builtin -fno-exceptions -fno-stack-protector -O0 -m32 -fno-rtti -nostdlib -ffreestanding
