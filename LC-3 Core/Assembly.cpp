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
	
}


Assembly::~Assembly()
{
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
			if (!(IsR(temp) || temp == cmt)) buffer[length++] = sp;
		}
		buffer[length++] = temp;
		temp = fgetc(p);
	}
	buffer[length] = '\0';
}