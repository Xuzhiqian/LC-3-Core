#pragma once
#include "stdafx.h"
class Memory
{
public:
	Memory();
	~Memory();
	word Fetch(word address);  //Merged Loading Function
	void Store(word address, word value);   //Merged Storing Function

	word GetMDR();
	void SetMDR(word value);
	void SetMAR(word value);
	void FetchMemory();
	void StoreMemory();
	void Clear();
private:
	word MDR;
	word MAR;
	word memory[SPACE];
	void BatchAssign(word start, word end, const word value[]);
};
