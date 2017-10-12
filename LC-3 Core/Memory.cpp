#include "stdafx.h"
#include "Memory.h"


Memory::Memory()
{
	Clear();
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
