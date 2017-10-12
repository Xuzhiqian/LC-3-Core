#pragma once
#include "stdafx.h"
class ProcessingUnit
{
public:
	ProcessingUnit();
	~ProcessingUnit();
	word GetRegister(word X, int start, int end);
	void StoreRegister(word X, int start, int end, word value);
	word Add(word a, word b);
	word Not(word a);
	word And(word a, word b);

	//word Subtract(word a,word b);
	//word Multiply(word a,word b);
	//......

	word Decode(word X, int start, int end);
	void SetCC(word value);
	word SignExtend(word value, int utmost);
	word ZeroExtend(word value, int utmost);

	word PSR;
private:
	word R[NUM_REGISTER];
	bool Bit(word value, int index);
};

