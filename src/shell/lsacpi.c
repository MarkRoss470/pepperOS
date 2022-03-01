#include <shell/lsacpi.h>

#include <drivers/ACPI.h>
#include <graphics.h>
#include <util/mem.h>
#include <util/strings.h>
#include <util/IO.h>
#include <shell/shell.h>
#include <screen.h>

static const char SDTs[][5] = {
	"APIC",
	"BERT",
	"CPEP",
	"DSDT",
	"ECDT",
	"EINJ",
	"ERST",
	"FACP",
	"FACS",
	"HEST",
	"MSCT",
	"MPST",
	"PMTT",
	"PSDT",
	"RASF",
	"SBST",
	"SLIT",
	"SRAT",
	"SSDT",
	"XSDT",
	"TEST"
};

static const char PowerProfiles[][20] = {
	"Unspecified",
	"Desktop",
	"Mobile",
	"Workstation",
	"Enterprise Server",
	"SOHO Server",
	"Applicance PC",
	"Performance Server"
};

int lsacpi(int argc, char *argv[])
{
	struct RSDPDescriptor* rsdp = locateRSDP();
	if(argc == 1)printf("RSDP is located at %#x. Revision: %d\n", (unsigned int)rsdp, rsdp->Revision);
	if(validateRSDP(rsdp))
	{
		if(argc == 1)printf("RSDP is valid\n");
	}
	else
	{
		printf("RSDP is invalid");
		return 1;
	}
	
	char OEMID[7];
	memcpy(OEMID, rsdp->OEMID, 6);
	OEMID[6] = 0;
	
	struct RSDT* rsdt = (struct RSDT*)rsdp->RsdtAddress;
	
	if(argc == 1)printf("OEMID: '%s'\nAddress: %#x\n", OEMID, rsdp->RsdtAddress);
	if(validateSDT((struct ACPISDTHeader*)rsdp->RsdtAddress))
	{
		if(argc == 1)printf("RSDT is valid\n");
	}
	else
	{
		printf("RSDT is invalid");
		return 1;
	}
	
	if(argc == 1)
	{
		for(int i = 0; i < (int)(sizeof(SDTs) / sizeof(SDTs[0])); i++)
		{
			struct ACPISDTHeader* sdt = locateSDT(rsdt, SDTs[i]);
			if(sdt)
			{
				printf("%s:%#x  ", SDTs[i], sdt);
			}
		}
	}
	else
	{
		if(strlen(argv[1]) != 4)
		{
			printf("Table ID must be 4 letters long");
			return 1;
		}
		struct ACPISDTHeader* sdt = locateSDT(rsdt, argv[1]);
		if(sdt == NULL)
		{
			printf("Table not found");
			return 1;
		}
		if(!validateSDT(sdt))
		{
			printf("Table is invalid");
			return 1;
		}
		
		if(strcmp(argv[1], "APIC"))
		{
			struct MADT* madt = (struct MADT*)sdt;
			printf("Length is %d. Local APIC at %#x\n", madt->h.Length, madt->LocalAPICAddress);
			if(madt->Flags & 1)printf("legacy 8259s installed. ");
			if(madt->Flags & 2)printf("8259 PIC interrupts must be masked. ");
			if(madt->Flags & 3)printf("\n");
			if(argc == 3)return 0;
			for(int i = 0; i < (int)(madt->h.Length - sizeof(struct MADT)); i += ((struct MADTRecordHeader*)(madt->Records + i))->Length)
			{
				switch(madt->Records[i])
				{
					case(0):
						{
						printf("0: Processor Local APIC\nProc ID: %d, APIC ID: %d\n", madt->Records[i+2], madt->Records[i+3]);
						if(madt->Records[i+7] & 1)printf("Enabled. ");
						if(madt->Records[i+7] & 2)printf("Online capable. ");
						if(madt->Records[i+7] & 3)printf("\n");
						break;
						}
					case(1):
						{
						printf("1: I/O APIC Address %#x\nID: %d, GSIB: %d\n", *(uint32_t*)(madt->Records+i+4), madt->Records[i+2], *(uint32_t*)(madt->Records+i+8));
						break;
						}
					case(2):
						{
						if(!(madt->Records[i+2] | madt->Records[i+3] | *(uint32_t*)(madt->Records+i+4)))break;
						printf("2: IO/APIC IS Override\nBus:%d,IRQ:%d,GSI:%d\n", madt->Records[i+2], madt->Records[i+3], *(uint32_t*)(madt->Records+i+4));
						break;
						}
					case(3):
						{
						printf("3: IO/APIC Non-maskable interrupt source\nNMI Source: %d, GSI: %d\n", madt->Records[i+2], *(uint32_t*)(madt->Records+i+6));
						break;
						}
					case(4):
						{
						if(!(madt->Records[i+2] | madt->Records[i+5]))break;
						printf("4: Local APIC Non-maskable interrupts\nProc ID: %d, LINT#: %d\n", madt->Records[i+2], madt->Records[i+5]);
						break;
						}
					default:
						printf("Unknown record type %d", madt->Records[i]);
						return 1;
				}
			}
		}
		else if(strcmp(argv[1], "FACP"))
		{
			struct FADT* fadt = (struct FADT*)sdt;
			printf("FACS:%#x. DSTD:%#x\n", fadt->FirmwareCtrl, fadt->Dsdt);
			if(fadt->PreferredPowerManagementProfile > 7)printf("Unknown power management profile\n");
			else printf("Power management profile: %s\n", PowerProfiles[fadt->PreferredPowerManagementProfile]);
			printf("SCI Interrupt: %d, SMI Command Port: %#x", fadt->SCI_Interrupt, fadt->SMI_CommandPort);
		}
		else
		{
			printf("Table parser not implemented");
			return 1;
		}
	}
	return 0;
}

int enableacpi(int argc, char *argv[])
{
	enableACPI();
	return 0;
}

int poweroff(int argc, char *argv[])
{
	printf("Don't try it\n");
	return(1);

	if(!ACPIEnabled)
	{
		printf("Enable ACPI first!");
		return 1;
	}
	
	
	struct RSDT* rsdt = (struct RSDT*)locateRSDP()->RsdtAddress;
	struct FADT* fadt = (struct FADT*)locateSDT(rsdt, "FACP");
	struct ACPISDTHeader* sdt = (struct ACPISDTHeader*)fadt->Dsdt;
	uint8_t* intptr = (uint8_t*)fadt->Dsdt;
	
	bool found = false;
	int i = sizeof(struct ACPISDTHeader);
	for(;i < (int)sdt->Length; i++)
	{
		if(intptr[i] == 0x5f && intptr[i+1] == 0x53 && intptr[i+2] == 0x35)
		{
			found=true;
			break;
		}
	}
	if(!found)
	{
		printf("\\_S5 object not found");
		return 1;
	}
	
	if(!((intptr[i-1] == 0x08 || (intptr[i-2] == 0x08 && intptr[i-1] == '\\')) && intptr[i+4] == 0x12))
	{
		printf("Invalid AML structure");
		return 1;
	}
	
	uint16_t SLP_TYPa;
	uint16_t SLP_TYPb;
	
	i += 5;
	i += (intptr[i] >> 6) + 2;
	if(intptr[i] ==  0x0a)i++;
	SLP_TYPa = intptr[i]<<10;
	i++;
	if(intptr[i] == 0x0a)i++;
	SLP_TYPb = intptr[i]<<10;
	
	uint16_t SLP_EN = fadt->PM1bControlBlock & (1 << 13);
	
	printf("%#x %#x %#x %d", SLP_TYPa, fadt->PM1aControlBlock, fadt->PM1bControlBlock, SLP_EN);
	screen_swap();
	outw_p(fadt->PM1aControlBlock, SLP_TYPa | SLP_EN);
	return 0;
}

int lsapic(int argc, char* argv[])
{
	struct RSDPDescriptor* rsdp = locateRSDP();
	struct RSDT* rsdt = (struct RSDT*)rsdp->RsdtAddress;
	struct ACPISDTHeader* sdt = locateSDT(rsdt, "APIC");
	struct MADT* madt = (struct MADT*)sdt;
	
	if(argc > 1 && uatoi(argv[1]) != -1)
	{
		for(int i = 0; i < (int)(madt->h.Length - sizeof(struct MADT)); i += ((struct MADTRecordHeader*)(madt->Records + i))->Length)
		{
			if(madt->Records[i] == 1)
			{
				printf("ID:%d, GSIB:%d, Address:%#x\n", madt->Records[i+2], *(uint32_t*)(madt->Records+i+8), *(uint32_t*)(madt->Records+i+4));
				uint32_t* apic = (uint32_t*)(*(uint32_t*)(madt->Records+i+4));
				uint32_t volatile* IOREGSEL = apic;
				//This +4 is actually adding 0x10 as it is a type of with 4
				uint32_t volatile* IOREGWIN = apic + 4;
				
				*IOREGSEL = 0;
				uint8_t id = ((*IOREGWIN) >> 24) & 0xf;
				*IOREGSEL = 1;
				//VER: version   MRE: Max Redirection Entry
				uint8_t version = *IOREGWIN & 0xff;
				uint16_t MRE = *IOREGWIN >> 16 & 0xff;
				*IOREGSEL = 2;
				uint8_t ArbID = *IOREGWIN >> 24 & 0xf;
				
				if(argc == 1)
				{
					printf("ID: %d\n", id);
					printf("VER: %d  MRE: %d\n", version, MRE);
					printf("Arbitration ID: %d\n", ArbID);
				}
				
				if(argc == 2)
				{
					int intNo = uatoi(argv[1]);
					if(intNo == -1)
					{
						printf("Failed to parse int no");
						return 1;
					}
					if(intNo > MRE)
					{
						printf("Int no too large");
						return 1;
					}
					*IOREGSEL = 0x10 + intNo * 2;
					printf("%#x\n", *IOREGWIN);
					*IOREGSEL = 0x11 + intNo * 2;
					printf("%#x\n", *IOREGWIN);
				}
			}
		}
	}
	else if(strcmp(argv[1], "local"))
	{
		uint32_t* localAPIC = (uint32_t*)madt->LocalAPICAddress;
		
	}
	return 0;
}





















