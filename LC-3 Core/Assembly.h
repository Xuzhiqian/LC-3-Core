#pragma once
#include "Specification.h"
#include <regex>
#include <string>
#include <map>
#include <queue>
#define ERROR -0x1ffff
using namespace std;

class Assembly
{
public:
	Assembly();
	~Assembly();
	word Assemble(FILE *p);
	map<word,word> target;

	
	void Filter(FILE *p);
	void Initialize();
	int First();
	int Second(int FA);
	word parseNum(string c);
	int parseReg(string &c);
	word parseOffset(string op, word PC, int width);
	word replaceLabel(word PC, string op, int width);
	word complement(int w, word n);
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
	regex number;
	regex stringz;
	regex br;
	map<string, regex> syntax;

	word Translate(word ins, string instr,string op,word PC);
private:
	
	
	
};

