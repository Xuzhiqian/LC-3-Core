#pragma once
#include "ProcessingUnit.h"
#include "Memory.h"
class ControlUnit
{
public:
	ControlUnit(ProcessingUnit *pointer_p, Memory *pointer_m);
	~ControlUnit();
	void Execute();
	void SetPC(word value);
private:
	ProcessingUnit *p;
	Memory *m;
	word PC;
	word IR;
	bool Bit(word value, int index);

	//Instruction Set Definition
	void ADD(word value);
	void AND(word value);
	void BR(word value);
	void JMP(word value);
	void JSR(word value);
	void LD(word value);
	void LDI(word value);
	void LDR(word value);
	void LEA(word value);
	void NOT(word value);
	void RTI(word value);
	void ST(word value);
	void STI(word value);
	void STR(word value);
	void TRAP(word value);
	void EXCP(word value);
};

