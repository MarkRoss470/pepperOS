#pragma once

int reserve_pmem(void* start, void* end);
void* get_mapping_vp(void* addr);
void* allocate_vmem(unsigned int pages);
int allocate_vmem_address(void* addr, unsigned int pages);

void memory_init();
