#include <drivers/memory.h>
#include <util/types.h>

void* get_mapping_vp(void* arrd)
{
	//TODO: parse GDT
	return NULL;
}

int reserve_pmem(void* start, void* end)
{
	//TODO: Decide on data structure (bit field or start/end address pairs?)
	return 1;
}

void memory_init()
{
	//BIOS data area
	reserve_pmem((void*)0xa0000, (void*)0xfffff);
}
