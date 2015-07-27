LD=ld
LDFLAG= -z max-page-size=0x1000 -T ./arch/$(ARCH)/linker.ld

SC=g++
SCFLAG= $(INCDIR) -ggdb -O3 -mcmodel=large -std=c++0x -w -trigraphs -fno-builtin -fno-exceptions -fno-stack-protector -fno-rtti -nostdlib -ffreestanding -mno-mmx -mno-sse -mno-sse2

ASM=g++
ASMFLAG= $(INCDIR) -g -mcmodel=large