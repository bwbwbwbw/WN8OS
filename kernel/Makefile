ARCH=x86_64
KERNEL=wn8os.bin
SDKDIR=../sdk
INCDIR= -I ./ -I ./core -I ./arch/$(ARCH)

include ./arch/$(ARCH)/config.make

include ./runtime/Makefile
include ./core/Makefile
include ./mm/Makefile
include ./arch/$(ARCH)/Makefile

all: $(KERNEL)

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAG) -o $@ $^
	cp $(KERNEL) $(SDKDIR)/boot/

%.o: %.cc
	$(SC) $(SCFLAG) -c $< -o $@

%.o: %.S
	$(ASM) $(ASMFLAG) -c $< -o $@

clean:
	rm -f $(OBJS) $(KERNEL)
