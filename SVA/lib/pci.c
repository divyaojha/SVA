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
		default:
			break;
                }
	}
	}else{
		/*TBD*/
		/*iowrite for rempaped mem*/;	
	}
}
