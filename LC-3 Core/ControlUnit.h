#pragma once
#include "ProcessingUnit.h"
#include "Memory.h"
class ControlUnit
{
public:
	ControlUnit(ProcessingUnit *pointer_p, Memory *pointer_m);
	~ControlUnit();
	void Process();
	void SetPC(word value);
private:
	ProcessingUnit *p;
	Memory *m;
	word PC;
	word IR;
	bool Bit(word value, int index);

	void Fetch();
	word Decode();
	word EvaluateAddress(word ins);
	void FetchOperands(word ins,word address,word &sop1,word &sop2);
	word Execute(word ins, word address,word sop1, word sop2);
	void StoreResult(word ins, word address, word sop1, word sop2,word result);

	void EXCP(word value);

	word sop1;
	word sop2;
};

