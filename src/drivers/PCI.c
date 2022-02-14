#include "PCI.h"
//0..7 register offset
//8..10 function number
//11..15 device number
//16..23 bus number
//24..30 reserved
//30 enable bit
#define CONFIG_ADDRESS 0xcf8
#define CONFIG_DATA 0xcfc

uint32_t pciConfigReadDouble(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
 
    /* create configuration address as per top comment */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    /* write out the address */
    outl(CONFIG_ADDRESS, address);
    /* read in the data */
    return inl(0xCFC);
}

uint16_t pciConfigReadWord (uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint16_t tmp = (uint16_t)((pciConfigReadDouble(bus, slot, func, offset) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}

uint16_t pciCheckVendor(uint8_t bus, uint8_t slot) {
    uint16_t vendor, device;
    /* try and read the first configuration register. Since there are no */
    /* vendors that == 0xFFFF, it must be a non-existent device. */
    if ((vendor = pciConfigReadWord(bus,slot,0,0)) != 0xFFFF) {
       device = pciConfigReadWord(bus,slot,0,2);
       
    } return (vendor);
}

//void enumerate_devices()
//{
	
//}
