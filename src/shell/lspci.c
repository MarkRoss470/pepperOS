#include <shell/lspci.h>

#include <drivers/PCI.h>
#include <graphics.h>
#include <util/strings.h>

char number[30];

void describeDevice(uint8_t bus, uint8_t device, uint8_t function)
{
	printf("\nDevice: %#x Vendor %#x", pciConfigReadWord(bus, device, function, DEVICE_ID), pciConfigReadWord(bus, device, function, VENDOR_ID));
	
	for(int i = 0; i <= 5; i++)
	{
		uint32_t BAR = pciConfigReadDouble(bus, device, function, HEADER_0_BAR0 + i * 4);
		if(BAR)
		{
			printf("\n%#x\nBAR %d: ", BAR, i);
			if(BAR & 0x1)
			{
				printf("Memory type %d %#x0", (BAR >> 1) & 3, BAR >> 4);
			}
			else
			{
				printf("IO %#x", BAR >> 2 << 2);
			}
		}	
	}
	putchar('\n');
}

void checkFunction(uint8_t bus, uint8_t device, uint8_t function)
{
	uint8_t baseClass;
	uint8_t subClass;
	uint8_t secondaryBus;

	uint16_t class_info = pciConfigReadWord(bus, device, function, CLASS_CODE);

	baseClass = class_info >> 8;
	subClass = class_info & 0xff;
	if( (baseClass == 0x06) && (subClass == 0x04) ) {
		secondaryBus = pciConfigReadWord(bus, device, function, HEADER_1_SECONDARY_BUS_NUMBER) >> 8;
		checkBus(secondaryBus);
	}
	else
	{
		#if false
		printf("%x:%x.%x base:%x sub:%s\n", bus, device, function, baseClass, subClass);
		#endif
		//if((baseClass == 0xc && subClass == 0x3))
		if(true)
		{
			describeDevice(bus, device, function);
		}
		
	}
}

void checkDevice(uint8_t bus, uint8_t device)
{
	uint8_t function = 0;
 
	uint16_t vendorID = pciConfigReadWord(bus, device, function, VENDOR_ID);
	if(vendorID == 0xFFFF) return;        // Device doesn't exist
	checkFunction(bus, device, function);
	uint16_t headerType = pciConfigReadWord(bus, device, function, HEADER_TYPE);
	if( (headerType & 0x80) != 0) {
		/* It is a multi-function device, so check remaining functions */
		for(function = 1; function < 8; function++) {
			if(pciConfigReadWord(bus, device, function, HEADER_TYPE) != 0xFFFF) {
				checkFunction(bus, device, function);
			}
		}
	}
	else
	{
	
	}
}

void checkBus(uint8_t bus)
{
	for(uint8_t device = 0; device < 32; device++)
	{
		checkDevice(bus, device);
	}
}

void checkAllBuses()
{
	uint8_t function;
	uint8_t bus;
	
	uint16_t headerType = pciConfigReadWord(0, 0, 0, HEADER_TYPE);
	
	if( (headerType & 0x80) == 0) 
	{
		/* Single PCI host controller */
		checkBus(0);
	}
	else
	{
		/* Multiple PCI host controllers */
		for(function = 0; function < 8; function++) 
		{
			if(pciConfigReadWord(0, 0, function, VENDOR_ID) != 0xFFFF) break;
			bus = function;
			checkBus(bus);
		}
	}
}

int lspci(int argc, char *argv[])
{
	checkAllBuses();
	puts("done");
	return(0);
}
