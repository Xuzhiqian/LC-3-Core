#pragma once
#include "LC-3 Specification.h"

const word TrapVectorTableConstant = 0xFD00;  //0x0000-0x00FF


#define TRAPSERVICEROUTINE_START 0x0020
#define TRAPSERVICEROUTINE_END 0x0025
const word TrapVectorTable[TRAPSERVICEROUTINE_END - TRAPSERVICEROUTINE_START + 1] = {
																					0x0400,			//GETC
																					0x0430,         //OUT
																					0x0450,			//PUTS
																					0x04A0,			//IN
																					0x04E0,			//PUTSP
																					0xFD70 };	    //HALT


#define GETC_START  0x0400
#define GETC_END 0x0407
const word GetcSPEC[GETC_END - GETC_START + 1] = {
													0x3E07,			//ST     R7, x0408    
													0xA004,			//R0<-memory[KBSR]
													0x07EE,			//BRZP   x0401
													0xA003,			//R0<-memory[KBDR]  
													0x2E03,			//LD     R7, x0408 
													0xC1C0,			//RET   
													0xFE00,			//where KBSR  resides        
													0xFE02 };		//where KBDR resides

#define OUT_START 0x0430
#define OUT_END 0x0439
const word OutSPEC[OUT_END - OUT_START + 1] = {
												0x3E0A,				//ST     R7, x043B 
												0x3208,				//ST     R1, x043A 
												0xA205,				//LDI    R1, x0438  
												0x07FE,				//BRZP   x0432 
												0xB004,				//STI    R0, x0439 
												0x2204,				//LD     R1, x043A 
												0x2E04,				//LD     R7, x043B 
												0xC1C0,				//RET   
												0xFE04,				//TRAP   x04 
												0xFE06,				//TRAP   x06  
};

#define PUTS_START 0x0450
#define PUTS_END 0x0463
const word PutsSPEC[PUTS_END - PUTS_START + 1] = {
													0x3E16,            //ST     R7, x0467
													0x3012,           //ST     R0, x0464
													0x3212,            //ST     R1, x0465
													0x3412,           //ST     R2, x0466
													0x6200,            //LDR    R1, R0, #0
													0x0405,            //BRZ    x045B
													0xA409,            //LDI    R2, x0460
													0x07FE,            //BRZP   x0456
													0xB208,            //STI    R1, x0461
													0x1021,            //ADD    R0, R0, #1
													0x0FF9,            //BRNZP  x0454
													0x2008,            //LD     R0, x0464
													0x2208,            //LD     R1, x0465
													0x2408,            //LD     R2, x0466
													0x2E08,            //LD     R7, x0467
													0xC1C0,            //RET
													0xFE04,            //TRAP   x04
													0xFE06,            //TRAP   x06
													0xF3FD,            //TRAP   xFD
													0xF3FE            //TRAP   xFE
};

#define IN_START 0x04A0
#define IN_END 0x04DF
const word InSPEC[IN_END - IN_START + 1] = {
	0x3E27,            //ST     R7, 0x04C8
	0x3625,            //ST     R3, 0x04C7
	0x3423,            //ST     R2, 0x04C6
	0x3221,            //ST     R1, 0x04C5
	0x201F,            //LD     R0, 0x04C4
	0x4813,            //JSR    0x04B9
	0xE222,            //LEA    R1, 0x04C9
	0x6040,            //LDR    R0, R1, #0
	0x0403,            //BRZ    0x04AC
	0x480F,            //JSR    0x04B9
	0x1261,            //ADD    R1, R1, #1
	0x0FFB,            //BRNZP  0x04A7
	0xA616,            //LDI    R3, 0x04C3
	0x07FE,            //BRZP   0x04AC
	0xA013,            //LDI    R0, 0x04C2
	0x1420,            //ADD    R2, R0, #0
	0x4808,            //JSR    0x04B9
	0x2012,            //LD     R0, 0x04C4
	0x4806,            //JSR    0x04B9
	0x10A0,            //ADD    R0, R2, #0
	0x2210,            //LD     R1, 0x04C5
	0x2410,            //LD     R2, 0x04C6
	0x2610,            //LD     R3, 0x04C7
	0x2E10,            // LD     R7, 0x04C8
	0xC1C0,           // RET
	0x3E05,            //ST     R7, 0x04BF
	0xA606,            //LDI    R3, 0x04C1
	0x07FD,            //BRZP   0x04B9
	0xB003,            //STI    R0, 0x04C0
	0x2E01,            //LD     R7, 0x04BF
	0xC1C0,            //RET
	0x0000,
	0xFE06,            //TRAP   0x06
	0xFE04,            //TRAP   0x04
	0xFE02,            //TRAP   0x02
	0xFE00,            //TRAP   0x00
	0x000A,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0049,
	0x006E,
	0x0070,
	0x0075,
	0x0074,
	0x0020,
	0x0061,
	0x0020,
	0x0063,
	0x0068,
	0x0061,
	0x0072,
	0x0061,
	0x0063,
	0x0074,
	0x0065,
	0x0072,
	0x003E,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
};


#define PUTSP_START 0x04E0
#define PUTSP_END 0x0503
const word PutspSPEC[PUTSP_END - PUTSP_START + 1] = {
	0x3E27,                   //ST     R7, 0x0508      
	0x3022,                   //ST     R0, 0x0504      
	0x3222,                   //ST     R1, 0x0505      
	0x3422,                   //ST     R2, 0x0506      
	0x3622,                   //ST     R3, 0x0507      
	0x1220,                   //ADD    R1, R0, #0     
	0x6040,                   //LDR    R0, R1, #0     
	0x0406,                   //BRZ    0x04EE          
	0x480D,                   //JSR    0x04F6          
	0x2418,                   //LD     R2, 0x0502      
	0x5002,                   //AND    R0, R0, R2     
	0x0402,                   //BRZ    0x04EE          
	0x1261,                   //ADD    R1, R1, #1     
	0x0FF8,                   //BRNZP  0x04E6          
	0x2014,                   //LD     R0, 0x0503      
	0x4806,                   //JSR    0x04F6          
	0x2013,                   //LD     R0, 0x0504      
	0x2213,                   //LD     R1, 0x0505      
	0x2413,                   //LD     R2, 0x0506      
	0x2613,                   //LD     R3, 0x0507      
	0x2E13,                   //LD     R7, 0x0508      
	0xC1C0,                   //RET     
	0x3E06,                   //ST     R7, 0x04FD      
	0xA607,                   //LDI    R3, 0x04FF      
	0x0801,                   //BRN    0x04FA          
	0x0FFC,                   //BRNZP  0x04F6          
	0xB003,                   //STI    R0, 0x04FE      
	0x2E01,                   //LD     R7, 0x04FD      
	0xC1C0,                   //RET     
	0x0000,
	0xFE06,                   //TRAP   0x06
	0xFE04,                   //TRAP   0x04
	0xF3FD,                   //TRAP   0xFD
	0xF3FE,                   //TRAP   0xFE
	0xFF00,                   //TRAP   0x00
	0x000A,
};

#define HALT_START 0xFD70
#define HALT_END 0xFD7C
const word HALT_SPEC[HALT_END - HALT_START + 1] = {
	0x3E0E,
	0x320C,
	0x300A,
	0xE00C,
	0xF022,
	0xA22F,
	0x202F,
	0x5040,
	0xB02C,
	0x2003,
	0x2203,
	0x2E03,
	0xC1C0,
};

#define EXCP_ILLEGAL_START 0x1000
#define EXCP_ILLEGAL_END 0x103A
const word EXCP_SPEC[EXCP_ILLEGAL_END - EXCP_ILLEGAL_START + 1] = {
	0x1000,
	0x3008,
	0x2006,
	0xF021,
	0xE006,
	0xF022,
	0x2002,
	0xF021,
	0x8000,
	0x000A,
	0x0000,
	0x002D,
	0x002D,
	0x002D,
	0x002D,
	0x0041,
	0x006E,
	0x0020,
	0x0049,
	0x006C,
	0x006C,
	0x0065,
	0x0067,
	0x0061,
	0x006C,
	0x0020,
	0x004F,
	0x0070,
	0x0063,
	0x006F,
	0x0064,
	0x0065,
	0x0020,
	0x0045,
	0x0078,
	0x0063,
	0x0065,
	0x0070,
	0x0074,
	0x0069,
	0x006F,
	0x006E,
	0x0020,
	0x0068,
	0x0061,
	0x0073,
	0x0020,
	0x006F,
	0x0063,
	0x0063,
	0x0075,
	0x0072,
	0x0065,
	0x0064,
	0x002D,
	0x002D,
	0x002D,
	0x002D,
	0x0000
};


#define EXCP_PRIVILEGE_START 0x1600
#define EXCP_PRIVILEGE_END 0x103A
const word EXCP_PRIVILEGE_SPEC[EXCP_PRIVILEGE_END - EXCP_ILLEGAL_START + 1] = {
	0x1600,
	0x3008,
	0x2006,
	0xF021,
	0xE006,
	0xF022,
	0x2002,
	0xF021,
	0x8000,
	0x000A,
	0x0000,
	0x002D,
	0x002D,
	0x002D,
	0x002D,
	0x0041,
	0x0020,
	0x0050,
	0x0072,
	0x0069,
	0x0076,
	0x0069,
	0x006C,
	0x0065,
	0x0067,
	0x0065,
	0x0020,
	0x004D,
	0x006F,
	0x0064,
	0x0065,
	0x0020,
	0x0045,
	0x0078,
	0x0063,
	0x0065,
	0x0070,
	0x0074,
	0x0069,
	0x006F,
	0x006E,
	0x0020,
	0x0068,
	0x0061,
	0x0073,
	0x0020,
	0x006F,
	0x0063,
	0x0063,
	0x0075,
	0x0072,
	0x0065,
	0x0064,
	0x002D,
	0x002D,
	0x002D,
	0x002D,
	0x0000
};



#define SPECIAL2 0xFE04
#define SPECIAL2_VALUE 0x0000
#define SPECIAL3 0x0100
#define SPECIAL3_VALUE 0x1000
#define SPECIAL4 0x0101
#define SPECIAL_VALUE 0x1600 
