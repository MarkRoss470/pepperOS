#include "cpuinfo.h"

#include "../util/strings.h"
#include "../graphics.h"
#include <cpuid.h>

int cpuinfo(int argc, char *argv[])
{
	uint32_t string[4];
	char number[30];
	
	unsigned eax = 0, ebx = 0, ecx = 0, edx = 0;
	__get_cpuid(0, &eax, &ebx, &ecx, &edx);

	string[0] = ebx;
	string[1] = edx;
	string[2] = ecx;
	string[3] = 0;
	
	puts("CPU vendor string: ");
	puts((char*) string);
	puts("\nMax cpuid eax value: ");
	puts(itoa(eax, number, 30));
	putchar('\n');
	
	__get_cpuid(1, &eax, &ebx, &ecx, &edx);
	
	puts("System ");
	if(edx & 1 << 9)puts("has");
	else puts("does not have");
	puts(" APIC\n");
	
	ecx = 0;
	eax = 1;
	__get_cpuid(1, &eax, &ebx, &ecx, &edx);
	ecx >>= 30;
	ecx &= 1;
	if(ecx)puts("system has rdrand support");
	else puts("system does not have rdrand support");
	
	return(0);
}
