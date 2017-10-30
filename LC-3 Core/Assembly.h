#pragma once
#include "Specification.h"
#include <regex>
#include <string>
#include <map>
#include <queue>
using namespace std;

class Assembly
{
public:
	Assembly();
	~Assembly();
	void Assemble(FILE *p);
	queue<word> target;

	
	void Filter(FILE *p);
	int First();
	int Second();
	word parseNum(string c);
	string upcase(string c);
	int match(int k,string c,bool IsCase);
	
	map<string, word> assembler;
	map<string, word> symboltable;
	string text,utext;
	queue<string> raw;
	queue<string[3]> table;

	regex label;
private:
	
	
	
};

