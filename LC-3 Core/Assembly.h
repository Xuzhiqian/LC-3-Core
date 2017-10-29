#pragma once
#include "Specification.h"
class Assembly
{
public:
	Assembly();
	~Assembly();
	void Assemble(FILE *p);
	word target[ASSEMBLY_MAXLENGTH];

	void Filter(FILE *p);
	char buffer[MAXINPUTCHAR];
	int length;
private:
	
	
	
};

