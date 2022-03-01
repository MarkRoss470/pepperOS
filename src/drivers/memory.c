#include <drivers/memory.h>

int reserve_pmem(void* start, void* end)
{
	
}

void init_mem()
{
	//BIOS data area
	reserve_pmem(0xa0000, 0xfffff);
}
