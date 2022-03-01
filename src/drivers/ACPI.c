#include <drivers/ACPI.h>
#include <util/strings.h>
#include <util/IO.h>
#include <graphics.h>
#include <timer.h>

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
	printf("FADT at %#x. PM1a control = %d\n", fadt, fadt->PM1aControlBlock);
	struct MADT* madt = (struct MADT*)locateSDT((struct RSDT*)locateRSDP()->RsdtAddress, "APIC");

	if((inw_p(fadt->PM1aControlBlock) & 1) == 0)
	{
		printf("ACPI already enabled\n");
		ACPIEnabled = true;
		return;
	}
	if(fadt->SMI_CommandPort == 0 || fadt->AcpiEnable == 0)
	{
		printf("No known way to enable ACPI\n");
		return;
	}
	outb_p(fadt->SMI_CommandPort,fadt->AcpiEnable);
	
	int i;
	for(i = 0; i < 30; i++)
	{
		if((inw(fadt->PM1aControlBlock) & 1) == 0)break;
		u64 current = timer_get();
		while(timer_get() < current + 100);
	}
	if(fadt->PM1bControlBlock != 0)
	{
		for(; i < 30; i++)
		{
			if((inw(fadt->PM1bControlBlock) & 1) == 0)break;
			u64 current = timer_get();
			while(timer_get() < current + 100);
		}
	}
	
	if(i > 300)
	{
		printf("ACPI failed to enable\n");
		return;
	}
	
	ACPIEnabled = true;
}















