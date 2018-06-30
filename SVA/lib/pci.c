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

static const unsigned int CONF1_ADDR_PORT = 0x0cf8;
static const unsigned int CONF1_DATA_PORT = 0x0cfc;
static const unsigned int PCI_BUSMAX = 255;     /* highest supported bus number */
static const unsigned int PCI_SLOTMAX = 31;      /* highest supported slot number */
static const unsigned int PCI_FUNCMAX = 7;       /* highest supported function number */
static const unsigned int PCI_REGMAX = 255;     /* highest supported config register addr. */


/*
 * Intrinsic: sva_iowrite()
 *
 * Descrption:
 * This intrinsic permits the system system software to write
 * to an IO port or memory mapped IO.
 *  
 * Note: write to memory mapped IO needs implementation 
 */
void sva_iowrite(int port, int data, int type){

	unsigned bus;
	unsigned slot;
	unsigned func;
	unsigned reg;
	int temp_port = port;
        
	if (port <= 0xffff) {
		/* If the given port is an IO port address-
		 * extract the reg, func, slot, bus number
		 * by shifting bits in the port address.
		 */

		reg  = temp_port & 0xFF;
		func = (temp_port >> 8) & 0x7;
		slot = (temp_port >> 3) & 0x1FF;
		bus  = (temp_port >> 5) & 0xFF;

		/* The following check is to return without
		 * doing any IO write operation. We check if
		 * the write request is to the IO ports assigned   
		 * to Source Address Decoder (SAD) or Target 
		 * Address Decoder (TAD). Writing to these ports 
		 * can reconfigure the DRAM mapping available on 
		 * a server, which in turn can allow illegal 
		 * access into memory regions.
		 */
	 	
		if ((slot == 15) && (func == 4) && (reg >=0x60) && (reg <=0xFC)) { /*SAD*/
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
