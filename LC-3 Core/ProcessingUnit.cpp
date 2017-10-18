#include "stdafx.h"
#include "ProcessingUnit.h"


ProcessingUnit::ProcessingUnit()
{
	for (int i = 0; i < NUM_REGISTER; i++) R[i] = 0;
	PSR = 0x8002;
}


ProcessingUnit::~ProcessingUnit()
{
}

word ProcessingUnit::Decode(word X, int start, int end) {
	if (end - start < 0) return 0;
	word temp = 0;
	for (int i = start; i <= end; i++)
		temp += 1 << i;
	temp = (temp & X) >> start;
	return temp;
}
word ProcessingUnit::GetRegister(word X, int start, int end) {
	return R[Decode(X, start, end)];
}

void ProcessingUnit::StoreRegister(word X, int start, int end, word value) {
	R[Decode(X, start, end)] = value;
}

word ProcessingUnit::And(word a, word b) {
	return Decode(a & b, 0, WORD_LENGTH - 1);
}

word ProcessingUnit::Not(word a) {
	return ~a;
}

word ProcessingUnit::Add(word a, word b) {
	return Decode(a + b,0,WORD_LENGTH-1);
}

void ProcessingUnit::SetCC(word value) {
	if (Decode((value >> (WORD_LENGTH - 1)),0,WORD_LENGTH-1) % 2 == 1) {
		PSR |= 4;									//PSR[2] is set since N=1
		PSR &= (SPACE - 2);
		PSR &= (SPACE - 1);
	}
	else if (Decode((value & (SPACE - 1)),0,WORD_LENGTH-1) == 0) {
		PSR &= (SPACE - 4);
		PSR |= 2;									//PSR[1] is set since Z=1
		PSR &= (SPACE - 1);
	}
	else {
		PSR &= (SPACE - 4);
		PSR &= (SPACE - 2);
		PSR |= 1;									//PSR[0] is set since P=1
	}
}

bool ProcessingUnit::Bit(word value, int index) {
	if (0 <= index && index < WORD_LENGTH)
		return (value >> index) % 2;
	else
		return 0;
}

word ProcessingUnit::SignExtend(word value, int utmost) {
	word temp = 0;
	if (Bit(value, utmost) == 1) {
		for (int i = utmost + 1; i < WORD_LENGTH; i++)
			temp += 1 << i;
		value |= temp;
	}
	else {
		for (int i = 0; i <= utmost; i++)
			temp += 1 << i;
		value &= temp;
	}
	return value;
}

word ProcessingUnit::ZeroExtend(word value, int utmost) {
	word temp = 0;
	for (int i = 0; i <= utmost; i++)
		temp += 1 << i;
	value &= temp;
	return value;
}