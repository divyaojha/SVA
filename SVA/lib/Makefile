#
# Makefile
#

#
# Include make.conf to configure the locations of the compiler
#
include ../../make.conf

CFLAGS := -I../include -O2 -fno-omit-frame-pointer -mno-aes -mno-avx -mcmodel=kernel -mno-red-zone -mno-mmx -msoft-float  -fno-asynchronous-unwind-tables -ffreestanding -Xclang -backend-option -Xclang -x86-add-cfi -no-integrated-as

OFILES = init.o secmem.o handlers.o mmu.o interrupt.o state.o debug.o \
         stateasm.o invoke.o invokeasm.o checks.o keys.o thread_stack.o \
         profile.o

libsva.a: $(OFILES)
	$(AR) -r $@ $(OFILES)

clean:
	rm -f *.a *.o *.i

