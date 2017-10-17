#include "stdafx.h"
#include "ControlUnit.h"


ControlUnit::ControlUnit(ProcessingUnit *pointer_p, Memory *pointer_m)
{
	p = pointer_p;
	m = pointer_m;
	PC = 0;
	IR = 0;
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

void ControlUnit::SetPC(word value) {
	PC = value;
}

void ControlUnit::Execute() {

	while (Bit(m->Fetch(MCR), 15)) {									 //while clock is enabled
		//Fetch
		m->SetMAR(PC);
		PC++;
		m->FetchMemory();
		IR = m->GetMDR();

		word I = p->ZeroExtend(p->Decode(IR, 12, 15),4);                     //if opcode 1101 is encountered, initiate and illegal opcode exception
		switch (I) {
			case 0:BR(IR); break;
			case 1:ADD(IR); break;
			case 2:LD(IR); break;
			case 3:ST(IR); break;
			case 4:JSR(IR); break;
			case 5:AND(IR); break;
			case 6:LDR(IR); break;
			case 7:STR(IR); break;
			case 8:RTI(IR); break;
			case 9:NOT(IR); break;
			case 10:LDI(IR); break;
			case 11:STI(IR); break;
			case 12:JMP(IR); break;
			case 13:EXCP(0xD000); break;
			case 14:LEA(IR); break;
			case 15:TRAP(IR); break;
		}

		if (Bit(m->Fetch(KBSR),15) == 1) {
			int c = getchar();
			m->Store(KBDR, c << (WORD_LENGTH-ASCII_LENGTH));
			m->Store(KBSR, 0);
		}

		if (Bit(m->Fetch(DSR), 15) == 1) {
			printf("%c", p->Decode(m->Fetch(DDR), 0, ASCII_LENGTH - 1));
			m->Store(DSR, 0);
		}
	}
}



//Instruction Set

void ControlUnit::ADD(word value) {
	word result;
	if (Bit(value, 5) == 0) {
		word SR1 = p->GetRegister(value, 6, 8);
		word SR2 = p->GetRegister(value, 0, 2);
		result = p->Add(SR1, SR2);
	}
	else {
		word SR1 = p->GetRegister(value, 6, 8);
		word imm5 = p->SignExtend(p->Decode(value, 0, 4), 4);
		result = p->Add(SR1, imm5);
	}
	p->StoreRegister(value, 9, 11, result);
	p->SetCC(result);
}


void ControlUnit::AND(word value) {
	word result;
	if (Bit(value, 5) == 0) {
		word SR1 = p->GetRegister(value, 6, 8);
		word SR2 = p->GetRegister(value, 0, 2);
		result = p->And(SR1, SR2);
	}
	else {
		word SR1 = p->GetRegister(value, 6, 8);
		word imm5 = p->SignExtend(p->Decode(value, 0, 4), 4);
		result = p->And(SR1, imm5);
	}
	p->StoreRegister(value, 9, 11, result);
	p->SetCC(result);
}

void ControlUnit::BR(word value) {
	if ((Bit(value, 11) && Bit(p->PSR, 2)) ||
		(Bit(value, 10) && Bit(p->PSR, 1)) ||
		(Bit(value, 9) && Bit(p->PSR, 0))) {
		PC = p->Add(PC, p->SignExtend(p->Decode(value, 0, 8), 8));
	}
}

void ControlUnit::JMP(word value) {
	PC = p->GetRegister(value, 6, 8);
}

void ControlUnit::JSR(word value) {
	word TEMP = PC;
	if (Bit(value, 11))
		PC = p->Add(PC, p->SignExtend(p->Decode(value, 0, 10), 10));
	else
		PC = p->GetRegister(value, 6, 8);
	p->StoreRegister(7, 0, 2, TEMP);
}

void ControlUnit::LD(word value) {
	word result = m->Fetch(p->Add(PC, p->SignExtend(p->Decode(value, 0, 8), 8)));
	p->StoreRegister(value, 9, 11, result);
	p->SetCC(result);
}

void ControlUnit::LDI(word value) {
	word result = m->Fetch(m->Fetch(p->Add(PC, p->SignExtend(p->Decode(value, 0, 8), 8))));
	p->StoreRegister(value, 9, 11, result);
	p->SetCC(result);
}

void ControlUnit::LDR(word value) {
	word result = m->Fetch(p->Add(p->GetRegister(value, 6, 8), p->SignExtend(p->Decode(value, 0, 5), 5)));
	p->StoreRegister(value, 9, 11, result);
	p->SetCC(result);
}

void ControlUnit::LEA(word value) {
	word result = p->Add(PC, p->SignExtend(p->Decode(value, 0, 8), 8));
	p->StoreRegister(value, 9, 11, result);
	p->SetCC(result);
}

void ControlUnit::NOT(word value) {
	word result = p->Not(p->GetRegister(value, 6, 8));
	p->StoreRegister(value, 9, 11, result);
	p->SetCC(result);
}

void ControlUnit::RTI(word value) {
	if (Bit(p->PSR, 15) == 0) {
		PC = m->Fetch(p->GetRegister(6, 0, 2));			//R6 is the SSP
		p->StoreRegister(6, 0, 2, p->Add(p->GetRegister(6, 0, 2), 1));
		word TEMP = m->Fetch(p->GetRegister(6, 0, 2));
		p->StoreRegister(6, 0, 2, p->Add(p->GetRegister(6, 0, 2), 1));
		p->PSR = TEMP;						//the privilege mode and condition codes of the interrupted process are restored
	}
	else EXCP(1);
}

void ControlUnit::ST(word value) {
	word address = p->Add(PC, p->SignExtend(p->Decode(value, 0, 8), 8));
	m->Store(address, p->GetRegister(value, 9, 11));
}

void ControlUnit::STI(word value) {
	word address = m->Fetch(p->Add(PC, p->SignExtend(p->Decode(value, 0, 8), 8)));
	m->Store(address, p->GetRegister(value, 9, 11));
}

void ControlUnit::STR(word value) {
	word address = p->Add(p->GetRegister(value, 6, 8), p->SignExtend(p->Decode(value, 0, 5), 5));
	m->Store(address, p->GetRegister(value, 9, 11));
}
void ControlUnit::TRAP(word value) {
	p->StoreRegister(7, 0, 2, PC);
	PC = m->Fetch(p->ZeroExtend(p->Decode(value, 0, 7), 7));
}

void ControlUnit::EXCP(word value) {
	
	p->StoreRegister(6, 0, 2, SSP);
	m->Store(p->GetRegister(6, 0, 2), p->PSR);
	p->PSR = p->PSR & (0x7FFF);
	p->StoreRegister(6, 0, 2, p->Add(p->GetRegister(6, 0, 2), -1));
	m->Store(p->GetRegister(6, 0, 2), PC);
	p->StoreRegister(6, 0, 2, p->Add(p->GetRegister(6, 0, 2), -1));
	if (Bit(value, 0) == 0)
		PC = m->Fetch(ILLEGAL_OPCODE_EXCEPTION_STRAT_ADDRESS_POINTER);
	else
		PC = m->Fetch(PRIVILEGE_MODE_EXCEPTION_STRAT_ADDRESS_POINTER);
}