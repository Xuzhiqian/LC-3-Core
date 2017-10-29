#pragma once

typedef int word;

#define WORD_LENGTH 16
#define SPACE 65536
#define NUM_REGISTER 8
#define REGISTER_ADDRESSABILITY 16
#define ASCII_LENGTH 8
#define WORD_BYTES 2


#define TRAP_VECTORTABLE_START 0x0000
#define TRAP_VECTORTABLE_END 0x00FF

#define INTERRUPT_VECTORTABLE_STRAT 0x0100
#define INTERRUPT_VECTORTABLE_END 0x01FF

#define OPERATINGSYSTEM_SUPERVISORSTACK_START 0x0200
#define OPERATINGSYSTEM_SUPERVISORSTACK_END 0x2FFF

#define USERSPACE_START 0x3000
#define USERSPACE_END 0xFDFF

#define DEVICEREGISTER_ADDRESSES_START 0xFE00
#define DEVICEREGISTER_ADDRESSES_END 0xFFFF
#define KBSR 0xFE00
#define KBDR 0xFE02
#define DSR 0xFE04
#define DDR 0xFE06
#define MCR 0xFFFE

#define SSP 0x2F80
#define USP 0x2FB0

#define ILLEGAL_OPCODE_EXCEPTION_STRAT_ADDRESS_POINTER 0x0100
#define PRIVILEGE_MODE_EXCEPTION_STRAT_ADDRESS_POINTER 0x0101

#define ASSEMBLY_MAXLENGTH 0x1000
#define MAXINPUTCHAR 0x8000