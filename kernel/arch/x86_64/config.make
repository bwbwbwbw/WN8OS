LD=ld
LDFLAG= -z max-page-size=0x1000 -nostdinc -T ./arch/$(ARCH)/linker.ld

SC=g++
SCFLAG= $(INCDIR) -g -O0 -mcmodel=large -std=c++0x -w -trigraphs -fno-builtin -fno-exceptions -fno-stack-protector -fno-rtti -nostdlib -ffreestanding -mno-mmx -mno-sse -mno-sse2
