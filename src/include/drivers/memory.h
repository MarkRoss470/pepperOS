#pragma once

int reserve_pmem(void* start, void* end);
void* get_mapping_vp(void* addr);
void* get_mapping_pv(void* addr);
void* allocate_vmem(unsigned int pages);

void init_mem();
