#include "stdafx.h"
#include "Assembly.h"
#define SPECIALINS -1

#define cmt ';'
#define sp ' '
#define IsSp(x) (((x)==32)||((x)==9))
#define IsRe(x) (((x)==10)||((x)==13))
#define IsR(x) ((IsSp(x))||(IsRe(x)))
using namespace std;


Assembly::Assembly()
{
	label = "^[a-z_A-Z][a-z0-9A-Z_]*$";
	number_hex = "^0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,4}$";
	number_dec = "^#?-?0*[0-9]{1,5}$";
	number = "^0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,4}|#?-?0*[0-9]{1,5}$";
	stringz = "\"([^\"]*)\"";
	br = "^BRN?Z?P?$";

	assembler.clear();
	assembler.insert(pair<string, word>("ADD", 0x1000));		syntax.insert(pair<string,regex>("ADD", "^[Rr][0-7],[Rr][0-7],([Rr][0-7]|0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,2}|#?-?0*[0-9]{1,2})$"));
	assembler.insert(pair<string, word>("AND", 0x5000));		syntax.insert(pair<string, regex>("AND", "^[Rr][0-7],[Rr][0-7],([Rr][0-7]|0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,2}|#?-?0*[0-9]{1,2})$"));
	assembler.insert(pair<string, word>("BR", 0x0000));			syntax.insert(pair<string, regex>("BR", "^0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*$"));
	assembler.insert(pair<string, word>("JMP", 0xC000));		syntax.insert(pair<string, regex>("JMP", "^[Rr][0-7]$"));
	assembler.insert(pair<string, word>("JSR", 0x4000));		syntax.insert(pair<string, regex>("JSR", "^0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,4}|[a-z_A-Z][a-z0-9A-Z_]*$"));
	assembler.insert(pair<string, word>("JSRR", 0x4000));		syntax.insert(pair<string, regex>("JSRR", "^[Rr][0-7]$"));
	assembler.insert(pair<string, word>("LD", 0x2000));			syntax.insert(pair<string, regex>("LD", "^[Rr][0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*)$"));
	assembler.insert(pair<string, word>("LDI", 0xA000));		syntax.insert(pair<string, regex>("LDI", "^[Rr][0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*)$"));
	assembler.insert(pair<string, word>("LDR", 0x6000));		syntax.insert(pair<string, regex>("LDR", "^[Rr][0-7],[Rr][0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,2}|#?-?0*[0-9]{1,2})$"));
	assembler.insert(pair<string, word>("LEA", 0xE000));		syntax.insert(pair<string, regex>("LEA", "^[Rr][0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*)$"));
	assembler.insert(pair<string, word>("NOT", 0x9000));		syntax.insert(pair<string, regex>("NOT", "^[Rr][0-7],[Rr][0-7]$"));
	assembler.insert(pair<string, word>("RET", 0xC1C0));		//no oprands
	assembler.insert(pair<string, word>("RTI", 0x8000));		//no oprands
	assembler.insert(pair<string, word>("ST", 0x3000));			syntax.insert(pair<string, regex>("ST", "^[Rr][0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*)$"));
	assembler.insert(pair<string, word>("STI", 0xB000));		syntax.insert(pair<string, regex>("STI", "^[Rr][0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*)$"));
	assembler.insert(pair<string, word>("STR", 0x7000));		syntax.insert(pair<string, regex>("STR", "^[Rr][0-7],[Rr][0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,2}|#?-?0*[0-9]{1,2})$"));
	assembler.insert(pair<string, word>("TRAP", 0xF000));		syntax.insert(pair<string, regex>("TRAP", "^0?[xX]0*[0-9AaBbCcDdEeFf]{1,2}|#?0*[0-9]{1,3}$"));
	assembler.insert(pair<string, word>("HALT", 0xF025));		//no oprands
	assembler.insert(pair<string, word>(".FILL", SPECIALINS));	syntax.insert(pair<string, regex>(".FILL", "0?[xX]0*[0-9AaBbCcDdEeFf]{1,4}|#?0*[0-9]{1,5}"));
	assembler.insert(pair<string, word>(".BLKW", SPECIALINS));
	assembler.insert(pair<string, word>(".STRINGZ", SPECIALINS));
	symboltable.clear();
	text.clear();
	target.clear();
	raw = queue<string>();
	table = queue<line>();
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
int Assembly::parseReg(string &c) {
	int s;
	if (c.find("R", 0) != string::npos)
		s = c.find("R", 0) + 1;
	else
		s = c.find("r", 0) + 1;
	int r = c.at(s) - '0';
	if (s + 1 < c.length())
		if (c[s + 1] == ',')
			c = c.substr(s + 2);
	return r;
}
bool isSafe(word num, int width) {
	width--;
	if ((num >= -(1 << width)) && (num < 1 << width))
		return 1;
	else
		return 0;
}
word Assembly::complement(int w, word n) {
	if (!isSafe(n, w)) return ERROR;
	if (n >= 0) return n;
	return (1 << w) + n;
}
word Assembly::parseOffset(string op, word PC, int width) {
	word offset;
	if (regex_match(op, number))
		offset = complement(width, parseNum(op));
	else
		offset = complement(width, replaceLabel(PC, op, width));
	if (offset == ERROR) return ERROR;
	else return offset;
}
word Assembly::parseNum(string c) {
	word r=0;
	int point = 0;
	while (point < c.length() - 1 && c[point] != '0'&&c[point] != 'x'&&c[point] != 'X'&&c[point] != '#') point++;
	c = c.substr(point);
	if (regex_match(c,number_hex))
	{
		bool IsNeg = false;
		if (c.at(0) != '0') c = '0' + c;
		if (c.find('-', 0) != string::npos) {
			c.erase(c.find('-', 0), 1);
			IsNeg = true;
		}
		sscanf(c.c_str(), "%x", &r);
		if (IsNeg) r = -r;
	}
	else if (regex_match(c, number_dec)) {
		if (c.at(0) == '#') c.erase(0, 1);
		sscanf(c.c_str(), "%d", &r);
	}
	else return ERROR;
	return r;
}
bool Assembly::Assemble(FILE *p) {
	Filter(p);
	int start = First();
	if (start == ERROR) return 0;
	if (Second(start) == ERROR) return 0;
	return 1;
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
	if (!match(0, ".ORIG", false)) return ERROR;
	int end = utext.find(".END", 0);
	if (end == string::npos) return ERROR;
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

	raw.pop();    //pop ".ORIG"
	word FA = parseNum(raw.front());				//Load FA as the first address of the program
	if (FA < 0 || FA>SPACE) return ERROR;
	raw.pop();

	line s;
	word LC = FA;
	while (!raw.empty()) {
		if (assembler.count(upcase(raw.front())) == 0) {
			if (!regex_match(upcase(raw.front()), br)) {
				s.label = raw.front();
				if (!regex_match(s.label, label))
					return ERROR;
				if (symboltable.count(s.label))
					return ERROR;
				else
					symboltable.insert(pair<string, word>(s.label, LC));
				raw.pop();
			}
			else s.label = "";
		}
		else s.label = "";

		if (!raw.empty()) {
			s.instr = upcase(raw.front());
			if (s.instr.find("BR", 0) == 0)
				if (!regex_match(s.instr, br))
					return ERROR;
			raw.pop();
		}
		else
			s.instr = "";

		if (s.instr == "HALT" || s.instr == "RTI" || s.instr == "RET")
			s.oprnd = "";
		else
			if (!raw.empty()) {
				s.oprnd = raw.front();
				raw.pop();
			}
			else s.oprnd = "";
			table.push(s);

			if (s.instr == ".STRINGZ")
				if (regex_match(s.oprnd, stringz))
					LC += s.oprnd.length() - 1;
				else
					return ERROR;
			else if (s.instr == ".BLKW")
				if (regex_match(s.oprnd, number_dec) || regex_match(s.oprnd, number_hex)) {
					word num = parseNum(s.oprnd);
					if (num >= 0 && num < SPACE)
						LC += num;
					else return ERROR;
				}
				else return ERROR;
			else LC++;
	}
	return FA;
}

int Assembly::Second(int PC) {
	map<string, regex>::iterator it;
	map<string, word>::iterator at;
	word CUR = 0;
	word result = 0;
	string instr, oprnd;
	while (!table.empty()) {
		CUR = PC++;
		instr = table.front().instr;
		oprnd = table.front().oprnd;
		table.pop();
		it = syntax.find(instr.find("BR", 0) != string::npos ? "BR" : instr);
		at = assembler.find(instr.find("BR", 0) != string::npos ? "BR" : instr);
		if (it != syntax.end()) {
			if (regex_match(oprnd, it->second)) {
				result = Translate(at->second, instr, oprnd, PC);
				if (result == ERROR) return ERROR;
				target.insert(pair<word, word>(CUR, result));
			}
			else return ERROR;
		}
		else {
			word temp = assembler.find(instr)->second;
			if (temp != SPECIALINS)
				target.insert(pair<word, word>(CUR, temp));
			else {													//.BLKW / .STRINGZ
				if (instr == ".BLKW") {
					int blocks = parseNum(oprnd);
					for (int i = 0; i < blocks; i++) {
						target.insert(pair<word, word>(CUR, 0));
						if (i != blocks - 1) CUR = PC++;
					}
				}
				if (instr == ".STRINGZ") {
					oprnd = oprnd.substr(1, oprnd.length() - 2);
					for (int i = 0; i < oprnd.length(); i++) {
						target.insert(pair<word, word>(CUR, oprnd.at(i)));
						CUR = PC++;
					}
					target.insert(pair<word, word>(CUR, 0));
				}
			}
		}

	}
	return 0;
}


word Assembly::replaceLabel(word PC, string op, int width) {
	if (regex_match(op, number))
		return parseNum(op);
	else
		return symboltable.find(op)->second - PC;
}
word Assembly::Translate(word ins,string instr,string op,word PC) {
	int r1, r2, r3;
	word offset, imm,trapvect;
	int n, z, p;
	if (instr == ".FILL") {
		word value = parseNum(op);
		if (value <= SPACE)
			return value;
		else
			return ERROR;
	}
	switch (ins) {
	case 0x1000:
	case 0x5000:
		r1 = parseReg(op);
		r2 = parseReg(op);
		if (regex_search(op, regex("R[0-7]"))) {
			r3 = parseReg(op);
			return ins + (r1 << 9) + (r2 << 6) + r3;
		}
		else {
			imm = complement(5, parseNum(op));
			if (imm==ERROR) return ERROR;
			return ins + (r1 << 9) + (r2 << 6) + (1 << 5) + imm;
		}
		break;

	case 0x0000:
		n = instr.find('N', 0) != string::npos;
		z= instr.find('Z', 0) != string::npos;
		p= instr.find('P', 0) != string::npos;
		if (!n && !z && !p) {								//BR is equivalent to BRnzp
			n = 1; z = 1; p = 1;
		}
		offset = parseOffset(op, PC, 9);
		if (offset == ERROR) return ERROR;
		return (n << 11) + (z << 10) + (p << 9) + offset;
		break;
		
	case 0xC000:
		r1 = parseReg(op);
		return ins + (r1 << 6);
		break;

	case 0x4000:
		if (instr == "JSR") {
			offset = parseOffset(op, PC, 11);
			if (offset == ERROR) return ERROR;
			return ins + (1 << 11) + offset;
		}
		else {
			r1 = parseReg(op);
			return ins + (r1 << 6);
		}
		break;

	case 0x2000:
	case 0xA000:
	case 0xE000:
	case 0x3000:
	case 0xB000:
		r1 = parseReg(op);
		offset = parseOffset(op, PC, 9);
		if (offset == ERROR) return ERROR;
		return ins + (r1 << 9) + offset;
		break;

	case 0x6000:
	case 0x7000:
		r1 = parseReg(op);
		r2 = parseReg(op);
		offset = parseOffset(op, PC, 6);
		if (offset == ERROR) return ERROR;
		return ins + (r1 << 9) + (r2 << 6) + offset;
		break;

	case 0x9000:
		r1 = parseReg(op);
		r2 = parseReg(op);
		return ins + (r1 << 9) + (r2 << 6);
		break;

	case 0xF000:
		trapvect = parseNum(op);
		return ins + trapvect;
	default:return 0;
	}
}

/*
assembler.insert(pair<string, word>("AND", 0x5000));		syntax.insert(pair<string, regex>("AND", "^R[0-7],R[0-7],(R[0-7]|0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,2}|#?-?0*[0-9]{1,2})$"));
assembler.insert(pair<string, word>("BR", 0x0000));			syntax.insert(pair<string, regex>("BR", "^0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*$"));
assembler.insert(pair<string, word>("JMP", 0xC000));		syntax.insert(pair<string, regex>("JMP", "^R[0-7]$"));
assembler.insert(pair<string, word>("JSR", 0x4000));		syntax.insert(pair<string, regex>("JSR", "^0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,4}|[a-z_A-Z][a-z0-9A-Z_]*$"));
assembler.insert(pair<string, word>("JSRR", 0x4000));		syntax.insert(pair<string, regex>("JSRR", "^R[0-7]$"));
assembler.insert(pair<string, word>("LD", 0x2000));			syntax.insert(pair<string, regex>("LD", "^R[0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*)$"));
assembler.insert(pair<string, word>("LDI", 0xA000));		syntax.insert(pair<string, regex>("LDI", "^R[0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*)$"));
assembler.insert(pair<string, word>("LDR", 0x6000));		syntax.insert(pair<string, regex>("LDR", "^R[0-7],R[0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,2}|#?-?0*[0-9]{1,2})$"));
assembler.insert(pair<string, word>("LEA", 0xE000));		syntax.insert(pair<string, regex>("LEA", "^R[0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*)$"));
assembler.insert(pair<string, word>("NOT", 0x9000));		syntax.insert(pair<string, regex>("NOT", "^R[0-7],R[0-7]$"));
assembler.insert(pair<string, word>("RET", 0xC1C0));		//no oprands
assembler.insert(pair<string, word>("RTI", 0x8000));		//no oprands
assembler.insert(pair<string, word>("ST", 0x3000));			syntax.insert(pair<string, regex>("ST", "^R[0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*)$"));
assembler.insert(pair<string, word>("STI", 0xB000));		syntax.insert(pair<string, regex>("STI", "^R[0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,3}|#?-?0*[0-9]{1,3}|[a-z_A-Z][a-z0-9A-Z_]*)$"));
assembler.insert(pair<string, word>("STR", 0x7000));		syntax.insert(pair<string, regex>("STR", "^R[0-7],R[0-7],(0?[xX]-?0*[0-9AaBbCcDdEeFf]{1,2}|#?-?0*[0-9]{1,2})$"));
assembler.insert(pair<string, word>("TRAP", 0xF000));		syntax.insert(pair<string, regex>("TRAP", "^0?[xX]0*[0-9AaBbCcDdEeFf]{1,2}|#?0*[0-9]{1,3}$"));
assembler.insert(pair<string, word>("HALT", 0xF025));		//no oprands
assembler.insert(pair<string, word>(".FILL", SPECIALINS));	syntax.insert(pair<string, regex>(".FILL", "0?[xX]0*[0-9AaBbCcDdEeFf]{1,4}|#?0*[0-9]{1,5}"));
assembler.insert(pair<string, word>(".BLKW", SPECIALINS));
assembler.insert(pair<string, word>(".STRINGZ", SPECIALINS));*/