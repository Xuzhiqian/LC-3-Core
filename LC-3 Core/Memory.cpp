#include "stdafx.h"
#include "Memory.h"
#include "Exception_routine.h"


#define SPECIAL3 0x0100
#define SPECIAL3_VALUE 0x1000
#define SPECIAL4 0x0101
#define SPECIAL_VALUE 0x1600 

Memory::Memory()
{
	ReInitialize();
}


Memory::~Memory()
{
}

void Memory::BatchAssign(word start, word end, const word value[]) {
	for (int i = start; i <= end; i++)
		memory[i] = value[i - start];
}
word Memory::Fetch(word address) {
	SetMAR(address);
	FetchMemory();
	return GetMDR();
}

word Memory::GetMDR() {
	return MDR;
}

void  Memory::SetMDR(word value) {
	MDR = value;
}

void Memory::ReInitialize() {
	MAR = 0;
	MDR = 0;

	
	FILE *k = fopen("kernal", "rb");
	if (k == NULL) {
		printf("Initialization Error! \n");
		system("pause");
		exit(0);
	}
	word v = 0;
	for (word i = 0; i < SPACE; i++) {
		fread(&v, WORD_BYTES, 1, k);
		memory[i] = v;
	}

	BatchAssign(EXCP_ILLEGAL_START, EXCP_ILLEGAL_END, EXCP_SPEC);
	BatchAssign(EXCP_PRIVILEGE_START, EXCP_PRIVILEGE_END, EXCP_PRIVILEGE_SPEC);

	fclose(k);
}

void Memory::SetMAR(word value) {
	MAR = value;
}

void Memory::FetchMemory() {
	if (0 <= MAR && MAR < SPACE) {
		MDR = memory[MAR];
		if (MAR == KBSR) {
			word c = 0;
			scanf_s("%c", c);
			memory[KBDR] = c;
		}
	}
	else
		MDR = 0;
}

void Memory::Store(word address, word value) {
	SetMAR(address);
	SetMDR(value);
	StoreMemory();
}
void Memory::StoreMemory() {
	memory[MAR] = MDR;

	if (memory[DDR] != 0) {
		printf("%c", memory[DDR]);
		memory[DDR] = 0;
	}
}
