#include "sva/state.h"

static __inline void
outb(u_int port, u_char data)
{
        __asm __volatile("outb %0, %w1" : : "a" (data), "Nd" (port));
}

static __inline void
outl(u_int port, u_int data)
{
        __asm __volatile("outl %0, %w1" : : "a" (data), "Nd" (port));
}
static __inline void
outw(u_int port, u_short data)
{
        __asm __volatile("outw %0, %w1" : : "a" (data), "Nd" (port));
}

#define CONF1_ADDR_PORT    0x0cf8
#define CONF1_DATA_PORT    0x0cfc
#define PCI_BUSMAX      255     /* highest supported bus number */
#define PCI_SLOTMAX     31      /* highest supported slot number */
#define PCI_FUNCMAX     7       /* highest supported function number */
#define PCI_REGMAX      255     /* highest supported config register addr. */

void sva_iowrite(int port, int data, int bytes){
        
	if(port <= 0xffff){
	if (port != 0) {
                switch (bytes) {
                case 1:
                        outb(port, data);
                        break;
                case 2:
                        outw(port, data);
                        break;
                case 4:
                        outl(port, data);
                        break;
                }
	}
	}else{
		/*TBD*/
		/*io write for rempaped mem*/;	
	}
}

void sva_pci_write(int bus, int slot, int func, int reg, int data, int bytes){

        int dataport = 0;
       int port;

/*TBD*/
/*if bus, device, fn is for router and the hotplug variable is not set, return*/

        if (bus <= PCI_BUSMAX && slot <= PCI_SLOTMAX && func <= PCI_FUNCMAX &&
            (unsigned)reg <= PCI_REGMAX && bytes != 3 &&
            (unsigned)bytes <= 4 && (reg & (bytes - 1)) == 0) {
                outl(CONF1_ADDR_PORT, (1 << 31) | (bus << 16) | (slot << 11)
                    | (func << 8) | (reg & ~0x03));
                dataport = CONF1_DATA_PORT + (reg & 0x03);
        }
       port = dataport;

	sva_iowrite(port, data, bytes);
}
