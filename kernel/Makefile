ARCH=x86_64
KERNEL=wn8os.bin
SDKDIR=../sdk
INCDIR= -I ./ -I ./arch/$(ARCH)

include ./arch/$(ARCH)/config.make
include ./arch/$(ARCH)/Makefile

all: $(KERNEL)

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAG) -o $@ $^
	cp $(KERNEL) $(SDKDIR)/boot/

%.o: %.cc
	$(SC) $(SCFLAG) -c $< -o  $@

%.o: %.S
	$(SC) $(SCFLAG) -c $< -o  $@

clean:
	rm -f $(OBJS) $(KERNEL)
