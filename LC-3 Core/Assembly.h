#pragma once
#include "Specification.h"
#include <regex>
#include <string>
#include <map>
#include <set>
using namespace std;

class Assembly
{
public:
	Assembly();
	~Assembly();
	void Assemble(FILE *p);
	set<word> target;

	
	void Filter(FILE *p);
	int First();
	int Second();
	word parseNum(string c);
	int match(int k,string c,bool IsCase);
	
	map<string, word> assembler;
	map<string, word> symboltable;
	string text,utext;
	set<string> queue;
	set<string[3]> table;

	regex label;
private:
	
	
	
};

