// LC-3 Core.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Memory.h"
#include "ControlUnit.h"
#include "ProcessingUnit.h"
#include "Assembly.h"
#include <iostream>
#include <regex>
using namespace std;

int main()
{
	Memory m;
	ProcessingUnit p;
	ControlUnit c(&p,&m);
	Assembly a;

	/*
	word v, start;
	scanf_s("%x", &start);
	c.SetPC(start);

	while (scanf_s("%x",&v))
		m.Store(start++,v);
	
	c.Process();
	*/


	FILE *ass = fopen("1.asm", "r");
	word start = a.Assemble(ass);
	if (start == ERROR) printf("Compile Error!\n");
	else {
		c.SetPC(start);
		fclose(ass);
		for (map<word, word>::iterator it = a.target.begin(); it != a.target.end(); ++it)
			m.LoadProgramPiece(it->first, it->second);
			//printf("%x     %x\n", it->first, it->second);
		c.Process();
	}
	system("pause");
    return 0;
}

