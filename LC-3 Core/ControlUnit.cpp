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

void ControlUnit::Fetch() {
		m->SetMAR(PC);
		PC++;
		m->FetchMemory();
		IR = m->GetMDR();
}

word ControlUnit::Decode() {
	return p->ZeroExtend(p->Decode(IR, 12, 15), 4);
}

word ControlUnit::EvaluateAddress(word ins) {
	switch (ins) {
		case 2:																				//LD
		case 10:																			//LDI
		case 14:																			//LEA
		case 3:																				//ST
		case 0:																				//BR
		case 11:return p->Add(PC, p->SignExtend(p->Decode(IR, 0, 8), 8)); break;			//STI
		case 7:																							//STR
		case 6:return p->Add(p->GetRegister(IR, 6, 8), p->SignExtend(p->Decode(IR, 0, 5), 5));	break;	//LDR
		case 15:return p->ZeroExtend(p->Decode(IR, 0, 7), 7); break;
		case 4:return p->Add(PC,p->SignExtend(p->Decode(IR, 0, 10), 10)); break;
		default:return -1;
	}
}

void ControlUnit::FetchOperands(word ins, word address, word &sop1, word &sop2) {
	sop1 = -1;
	sop2 = -1;
	switch (ins) {
	case 1:
	case 5:
		if (Bit(IR, 5) == 0)
		{
			sop1 = p->GetRegister(IR, 6, 8);
			sop2 = p->GetRegister(IR, 0, 2);
		}
		else
		{
			sop1 = p->GetRegister(IR, 6, 8);
			sop2 = p->SignExtend(p->Decode(IR, 0, 4), 4);
		}
		break;
	case 4:
		if (Bit(IR, 11) == 0)
			sop1 = p->GetRegister(IR, 6, 8);
		break;
	case 12:
	case 9:
		sop1 = p->GetRegister(IR, 6, 8);
		break;
	case 2:
	case 6:
	case 15:
		sop1 = m->Fetch(address);
		break;
	case 10:
		sop1 = m->Fetch(m->Fetch(address));
		break;
	case 3:
	case 11:
	case 7:
		sop1 = p->GetRegister(IR, 9, 11);
		break;
	default:break;
	}
}

word ControlUnit::Execute(word ins, word address,word sop1, word sop2) {
	word TEMP;
	switch (ins) {
	case 1:return p->Add(sop1, sop2); break;
	case 0:
		if ((Bit(p->PSR, 2) == 1 && Bit(IR, 11) == 1) ||
			(Bit(p->PSR, 1) == 1 && Bit(IR, 10) == 1) ||
			(Bit(p->PSR, 0) == 1 && Bit(IR, 9) == 1))
			PC = address;
		return -1;
	case 4:
		TEMP = PC;
		if (Bit(IR, 11) == 1)
			PC = address;
		else
			PC = sop1;
		p->StoreRegister(7, 0, 2, TEMP);
		return -1;
		break;
	case 5:return p->And(sop1, sop2); break;
	case 8:
		if (Bit(p->PSR, 15) == 0) {
			p->StoreRegister(6, 0, 2, p->Add(p->GetRegister(6, 0, 2), 1));
			PC = m->Fetch(p->GetRegister(6, 0, 2));			//R6 is the SSP
			p->StoreRegister(6, 0, 2, p->Add(p->GetRegister(6, 0, 2), 1));
			word TEMP = m->Fetch(p->GetRegister(6, 0, 2));
			p->PSR = TEMP;						//the privilege mode and condition codes of the interrupted process are restored
		}
		else EXCP(1);
		return -1;
		break;
	case 9:return p->Not(sop1); break;
	case 12:PC = sop1; return -1; break;
	case 13:EXCP(0xD000); return -1;  break;
	case 15:
		p->StoreRegister(7, 0, 2, PC);
		PC = sop1;
		return -1;
		break;
	default:return 0;
	}
}

void ControlUnit::StoreResult(word ins, word address, word sop1, word sop2,word result) {
	if (result == -1) return;
	switch (ins)
	{
	case 1:
	case 5:
	case 9:
		p->StoreRegister(IR, 9, 11, result);
		p->SetCC(result);
		break;
	case 2:
	case 6:
	case 10:
		p->StoreRegister(IR, 9, 11, sop1);
		p->SetCC(sop1);
		break;
	case 3:
	case 7:
		m->Store(address, sop1);
		break;
	case 11:
		m->Store(m->Fetch(address), sop1);
		break;
	case 14:
		p->StoreRegister(IR, 9, 11, address);
		p->SetCC(address);
		break;
	default:break;
	}
}
void ControlUnit::Process() {											//In one iteration
	while (Bit(m->Fetch(MCR), 15)) {									 //while clock is enabled
		
		Fetch();
		word I=Decode();
		word address = EvaluateAddress(I);
		FetchOperands(I, address, sop1, sop2);
		word result = Execute(I, address, sop1, sop2);
		StoreResult(I, address, sop1, sop2, result);
	}
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