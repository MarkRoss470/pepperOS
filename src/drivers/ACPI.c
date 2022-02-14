#include "ACPI.h"
#include "../util/strings.h"
#include "../util/IO.h"
#include "../graphics.h"

static const char* signature = "RSD PTR ";
bool ACPIEnabled = false;

struct RSDPDescriptor* locateRSDP()
{
	for(void* ptr = (void*) 0x000E0000; ptr < (void*)0x000FFFFF; ptr += 16)
	{
		for(int i = 0; i < 8; i++)
		{
			if(((struct RSDPDescriptor*)ptr)->Signature[i] != signature[i])goto after_return;
		}
		return((struct RSDPDescriptor*)ptr);
		after_return:;
	}
	return NULL;
}

bool validateRSDP(struct RSDPDescriptor* rsdp)
{
	uint8_t total = 0;
	for(unsigned int i = 0; i < sizeof(struct RSDPDescriptor); i++)
	{
		total += ((uint8_t*)rsdp)[i];
	}
	return(total == 0);
}

bool validateSDT(struct ACPISDTHeader* rsdt)
{
	uint8_t total = 0;
	for(unsigned int i = 0; i < rsdt->Length; i++)
	{
		total += ((uint8_t*)rsdt)[i];
	}
	return(total == 0);
}

struct ACPISDTHeader* locateSDT(struct RSDT* rsdt, const char* id)
{
	int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;
	for(int i = 0; i < entries; i++)
	{
		struct ACPISDTHeader *h = (struct ACPISDTHeader *) rsdt->PointerToOtherSDT[i];
		if (!strncmp(h->Signature, id, 4))return (struct ACPISDTHeader *) h;
	}
	return NULL;
}

void enableACPI()
{
	struct FADT* fadt = (struct FADT*)locateSDT((struct RSDT*)locateRSDP()->RsdtAddress, "FACP");
	struct MADT* madt = (struct MADT*)locateSDT((struct RSDT*)locateRSDP()->RsdtAddress, "APIC");
	outb(fadt->SMI_CommandPort,fadt->AcpiEnable);
	ACPIEnabled = true;
	*(uint32_t*)(madt->LocalAPICAddress + 0xF0) = *(uint32_t*)(madt->LocalAPICAddress + 0xF0) | 0x100;
}















