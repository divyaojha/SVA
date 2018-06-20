#include <sys/cdefs.h>
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

void sva_iowrite(int port, int data, int type){

	unsigned bus;
	unsigned slot;
	unsigned func;
	unsigned reg;
	
	int temp_port = port;

        
	if(port <= 0xffff){
	reg = temp_port & 0xFF;
	func = (temp_port >> 8) & 0x7;
	slot = (temp_port >> 3) & 0x1FF;
	bus = (temp_port >> 5) & 0xFF;
	if ((slot == 15) && (func == 4) && (reg >=0x60) && (reg <=0xFC)){ /*SAD*/
		return;
	}
	if ((slot >= 19) && (slot <= 22) && (func == 0) && (reg >=0x80) && (reg <=0xAC)){ /*TAD*/
		return;
	}
	
  
	if (port != 0) {
                switch (type) {
                case 1:
                        outb(port, data);
                        break;
                case 2:
                        outw(port, data);
                        break;
                case 4:
                        outl(port, data);
                        break;
/*		case 5:
			outsb(port, data);
			break;
		case 6:
			outsw(port, data);
			break;
		case 7:
			outsl(port, data);
			break;
*/
/* TBD string IO*/
		default:
			break;
                }
	}
	}else{
		/*TBD*/
		/*iowrite for rempaped mem*/;	
	}
}

#if 0
void sva_pci_write(int bus, int slot, int func, int reg, int data, int bytes){

        int dataport = 0;
       int port;

/*TBD*/
/*hotplug variable to be set and reset at the start and end of interrupt service
/*Get bus, slot, func, reg on server for 
*/

	if(bus==0 && slot==6 && func==0 && reg==8 && !hotplug){
		printf("I am not allowing any pikit******************:)\n");
		return;
	} else if(bus <= PCI_BUSMAX && slot <= PCI_SLOTMAX && func <= PCI_FUNCMAX &&
            (unsigned)reg <= PCI_REGMAX && bytes != 3 &&
            (unsigned)bytes <= 4 && (reg & (bytes - 1)) == 0) {
                outl(CONF1_ADDR_PORT, (1 << 31) | (bus << 16) | (slot << 11)
                    | (func << 8) | (reg & ~0x03));
                dataport = CONF1_DATA_PORT + (reg & 0x03);
        }
       port = dataport;

	sva_iowrite(port, data, bytes);
}
#endif
