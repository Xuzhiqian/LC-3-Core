#include "stdafx.h"
#include "Memory.h"
#include "Kernal.h"


Memory::Memory()
{
	ReInitialize();


	//BatchAssign(TRAPSERVICEROUTINE_START, TRAPSERVICEROUTINE_END, TrapVectorTable);
	//BatchAssign(GETC_START, GETC_END, GetcSPEC);
	//BatchAssign(OUT_START, OUT_END, OutSPEC);
	//BatchAssign(PUTS_START, PUTS_END, PutsSPEC);
	//BatchAssign(IN_START, IN_END, InSPEC);
	//BatchAssign(PUTSP_START, PUTSP_END,PutspSPEC);
	//BatchAssign(HALT_START, HALT_END, HALT_SPEC);
	
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
	BatchAssign(EXCP_ILLEGAL_START, EXCP_ILLEGAL_END, EXCP_SPEC);
	BatchAssign(EXCP_PRIVILEGE_START, EXCP_PRIVILEGE_END, EXCP_PRIVILEGE_SPEC);


	memory[SPECIAL2] = SPECIAL2_VALUE;
	memory[SPECIAL3] = SPECIAL3_VALUE;
	memory[MCR] = 0xFFFF;

}

void Memory::SetMAR(word value) {
	MAR = value;
}

void Memory::FetchMemory() {
	if (0 <= MAR && MAR < SPACE)
		MDR = memory[MAR];
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
}
