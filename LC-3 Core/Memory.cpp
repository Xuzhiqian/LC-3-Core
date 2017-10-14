#include "stdafx.h"
#include "Memory.h"
#include "Kernal.h"

void Memory::BatchAssign(word start, word end, const word value[]) {
	for (int i = start; i <= end; i++)
		memory[i] = value[i - start];
}
Memory::Memory()
{
	Clear();
	for (int i = TRAP_VECTORTABLE_START; i <= TRAP_VECTORTABLE_END; i++)
		memory[i] = TrapVectorTableConstant;

	BatchAssign(TRAPSERVICEROUTINE_START, TRAPSERVICEROUTINE_END, TrapVectorTable);
	BatchAssign(GETC_START, GETC_END, GetcSPEC);
	BatchAssign(OUT_START, OUT_END, OutSPEC);
	BatchAssign(PUTS_START, PUTS_END, PutsSPEC);
	BatchAssign(IN_START, IN_END, InSPEC);
	BatchAssign(PUTSP_START, PUTSP_END,PutspSPEC);
	
	memory[SPECIAL1] = SPECIAL1_VALUE;
	memory[SPECIAL2] = SPECIAL2_VALUE;
}


Memory::~Memory()
{
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

void Memory::Clear() {
	MAR = 0;
	MDR = 0;
	for (int i = 0; i < SPACE; i++) memory[i] = 0;
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
