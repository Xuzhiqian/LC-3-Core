#pragma once
#include "Specification.h"
#include <regex>
#include <string>
#include <map>
#include <queue>
#define ERROR -1
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
	struct line {
		string label;
		string instr;
		string oprnd;
	};
	queue<line> table;

	regex label;
	regex number_hex;
	regex number_dec;
	regex stringz;
	regex br;
	map<string, regex> syntax;
private:
	
	
	
};

