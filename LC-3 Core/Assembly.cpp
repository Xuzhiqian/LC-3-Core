#include "stdafx.h"
#include "Assembly.h"

#define cmt ';'
#define sp ' '
#define IsSp(x) (((x)==32)||((x)==9))
#define IsRe(x) (((x)==10)||((x)==13))
#define IsR(x) ((IsSp(x))||(IsRe(x)))
using namespace std;


Assembly::Assembly()
{
	assembler.clear();
	assembler.insert(pair<string, word>("ADD", 1));
	assembler.insert(pair<string, word>("AND", 5));
	assembler.insert(pair<string, word>("BR", 0));
	assembler.insert(pair<string, word>("JMP", 12));
	assembler.insert(pair<string, word>("JSR", 4));
	assembler.insert(pair<string, word>("JSRR", 4));
	assembler.insert(pair<string, word>("LD", 2));
	assembler.insert(pair<string, word>("LDI", 10));
	assembler.insert(pair<string, word>("LDR", 6));
	assembler.insert(pair<string, word>("LEA", 14));
	assembler.insert(pair<string, word>("NOT", 9));
	assembler.insert(pair<string, word>("RET", 12));			//no oprands
	assembler.insert(pair<string, word>("RTI", 8));				//no oprands
	assembler.insert(pair<string, word>("ST", 3));
	assembler.insert(pair<string, word>("STI", 11));
	assembler.insert(pair<string, word>("STR", 7));
	assembler.insert(pair<string, word>("TRAP", 15));
	assembler.insert(pair<string, word>(".ORIG", 20));
	assembler.insert(pair<string, word>("HALT", 21));				//no oprands
	assembler.insert(pair<string, word>(".FILL", 22));
	assembler.insert(pair<string, word>(".BLKW", 23));
	assembler.insert(pair<string, word>(".STRINGZ", 24));
	symboltable.clear();
	text.clear();
	raw = queue<string>();
	table = queue<string[3]>();
	target = queue<word>();

	label="^[a-z_A-Z][a-z0-9A-Z_]*$";
}


Assembly::~Assembly()
{
}

string Assembly::upcase(string c) {
	transform(c.begin(), c.end(), c.begin(), ::toupper);
	return c;
}

int Assembly::match(int k,string c,bool IsCase) {
	while (text.at(k) == ' '&&k < text.length()) k++;
	if (IsCase)
		if (text.find(c, k) != k) return 0;
		else return 1;
	else
		if (utext.find(upcase(c), k) != k) return 0;
		else return 1;
}

word Assembly::parseNum(string c) {
	word r=0;
	if ((c.find('x', 0) != string::npos) || (c.find('X', 0) != string::npos))
		sscanf(c.c_str(), "%x", &r);
	else
		sscanf(c.c_str(), "%d", &r);
	return r;
}
void Assembly::Assemble(FILE *p) {
	
}

void Assembly::Filter(FILE *p) {
	char temp = 0;
	int length = 0;
	temp = fgetc(p);
	while (!feof(p)) {	
		while (IsR(temp)||temp==cmt) {
			if (temp == cmt)
				while (!feof(p) && !IsRe(temp)) temp = fgetc(p);
			else
				while (!feof(p) && IsR(temp)) temp = fgetc(p);
			if (!(IsR(temp) || temp == cmt)) text.append(1,sp);
		}
		text.append(1,temp);
		temp = fgetc(p);
	}
	utext = upcase(text);
}

int Assembly::First() {
	if (!match(0, ".ORIG", false)) return -1;
	int end = utext.find(".END", 0);
	if (end == string::npos) return -1;
	text = text.substr(0, end);
	utext = utext.substr(0, end);


	int p = 0, q = 0;
	while (p < text.length())
	{
		q = text.find(' ', p);
		if (q == string::npos) q = text.length();
		if (p != q)
			raw.push(text.substr(p, q - p));
		p = q + 1;
	}

	string label,instr,oprand;
	while (!raw.empty()) {
		if (assembler.count(upcase(raw.front())) == 0) {
			label = raw.front();
			raw.pop();
		}
		else label = "";

		instr = upcase(raw.front());
		raw.pop();
		if (instr == "HALT" || instr == "RTI" || instr == "RET")
			oprand = "";
		else {
			oprand = upcase(raw.front());
			raw.pop();
		}
		table.push({ label,instr,oprand });
	}
	return 0;
}

