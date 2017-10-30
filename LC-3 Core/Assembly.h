#pragma once
#include "Specification.h"
#include <regex>
#include <string>
#include <map>
using namespace std;

class Assembly
{
public:
	Assembly();
	~Assembly();
	void Assemble(FILE *p);
	word target[ASSEMBLY_MAXROWS];

	
	void Filter(FILE *p);
	int First();
	int Second();
	word parseNum(string c);
	int match(int k,string c,bool IsCase);
	
	map<string, word> assembler;
	map<string, word> symboltable;
	string text,utext;
	string table[ASSEMBLY_MAXROWS][3];
	int num;

	regex label;
private:
	
	
	
};

