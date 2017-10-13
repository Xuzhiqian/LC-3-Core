#include "stdafx.h"
#include "ControlUnit.h"


ControlUnit::ControlUnit(ProcessingUnit *pointer_p, Memory *pointer_m)
{
	p = *pointer_p;
	m = *pointer_m;
}


ControlUnit::~ControlUnit()
{

}

bool ControlUnit::Bit(word value, int index) {
	if (0 <= index && index < WORD_LENGTH)
		return (value >> index) % 2;
	else
		return 0;
}

void ControlUnit::Execute() {

	//Fetch
	m.SetMAR(PC);
	PC++;
	m.FetchMemory();
	IR = m.GetMDR();

	word instruction = p.Decode(IR, 12, 15);                     //if opcode 1101 is encountered, initiate and illegal opcode exception
}



//Instruction Set

void ControlUnit::ADD(word value) {
	word result;
	if (Bit(value, 5) == 0) {
		word SR1 = p.GetRegister(value, 6, 8);
		word SR2 = p.GetRegister(value, 0, 2);
		result = p.Add(SR1, SR2);
	}
	else {
		word SR1 = p.GetRegister(value, 6, 8);
		word imm5 = p.SignExtend(p.Decode(value, 0, 4), 4);
		result = p.Add(SR1, imm5);
	}
	p.StoreRegister(value, 9, 11, result);
	p.SetCC(result);
}


void ControlUnit::AND(word value) {
	word result;
	if (Bit(value, 5) == 0) {
		word SR1 = p.GetRegister(value, 6, 8);
		word SR2 = p.GetRegister(value, 0, 2);
		result = p.And(SR1, SR2);
	}
	else {
		word SR1 = p.GetRegister(value, 6, 8);
		word imm5 = p.SignExtend(p.Decode(value, 0, 4), 4);
		result = p.And(SR1, imm5);
	}
	p.StoreRegister(value, 9, 11, result);
	p.SetCC(result);
}

void ControlUnit::BR(word value) {
	if ((Bit(value, 11) && Bit(p.PSR, 2)) ||
		(Bit(value, 10) && Bit(p.PSR, 1)) ||
		(Bit(value, 9) && Bit(p.PSR, 0))) {
		PC = p.Add(PC, p.SignExtend(p.Decode(value, 0, 8), 8));
	}
}

void ControlUnit::JMP(word value) {
	PC = p.GetRegister(value, 6, 8);
}

void ControlUnit::JSR(word value) {
	word TEMP = PC;
	if (Bit(value, 11))
		PC = p.Add(PC, p.SignExtend(p.Decode(value, 0, 10), 10));
	else
		PC = p.GetRegister(value, 6, 8);
	p.StoreRegister(7, 0, 2, TEMP);
}

void ControlUnit::LD(word value) {
	word result = m.Fetch(p.Add(PC, p.SignExtend(p.Decode(value, 0, 8), 8)));
	p.StoreRegister(value, 9, 11, result);
	p.SetCC(result);
}

void ControlUnit::LDI(word value) {
	word result = m.Fetch(m.Fetch(p.Add(PC, p.SignExtend(p.Decode(value, 0, 8), 8))));
	p.StoreRegister(value, 9, 11, result);
	p.SetCC(result);
}

void ControlUnit::LDR(word value) {
	word result = m.Fetch(p.Add(p.GetRegister(value, 6, 8), p.SignExtend(p.Decode(value, 0, 5), 5)));
	p.StoreRegister(value, 9, 11, result);
	p.SetCC(result);
}

void ControlUnit::LEA(word value) {
	word result = p.Add(PC, p.SignExtend(p.Decode(value, 0, 8), 8));
	p.StoreRegister(value, 9, 11, result);
	p.SetCC(result);
}

void ControlUnit::NOT(word value) {
	word result = p.Not(p.GetRegister(value, 6, 8));
	p.StoreRegister(value, 9, 11, result);
	p.SetCC(result);
}

void ControlUnit::RTI(word value) {
	if (Bit(p.PSR, 15) == 0) {
		PC = m.Fetch(p.GetRegister(6, 0, 2));			//R6 is the SSP
		p.StoreRegister(6, 0, 2, p.Add(p.GetRegister(6, 0, 2), 1));
		word TEMP = m.Fetch(p.GetRegister(6, 0, 2));
		p.StoreRegister(6, 0, 2, p.Add(p.GetRegister(6, 0, 2), 1));
		p.PSR = TEMP;						//the privilege mode and condition codes of the interrupted process are restored
	}
	else;									//Initiate a privilege mode exception;  Return PRIVILEGE MODE EXCEPTION;
}

void ControlUnit::ST(word value) {
	word address = p.Add(PC, p.SignExtend(p.Decode(value, 0, 8), 8));
	m.Store(address, p.GetRegister(value, 9, 11));
}

void ControlUnit::STI(word value) {
	word address = m.Fetch(p.Add(PC, p.SignExtend(p.Decode(value, 0, 8), 8)));
	m.Store(address, p.GetRegister(value, 9, 11));
}

void ControlUnit::STR(word value) {
	word address = p.Add(p.GetRegister(value, 6, 8), p.SignExtend(p.Decode(value, 0, 5), 5));
	m.Store(address, p.GetRegister(value, 9, 11));
}
void ControlUnit::TRAP(word value) {
	p.StoreRegister(7, 0, 2, PC);
	PC = m.Fetch(p.ZeroExtend(p.Decode(value, 0, 7), 7));
}