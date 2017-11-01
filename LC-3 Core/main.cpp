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
	a.Assemble(ass);
	fclose(ass);
	/*
	a.First();
	for (map<string, int>::iterator it = a.symboltable.begin(); it != a.symboltable.end(); ++it)
		cout << it->first << "		"<<it->second << endl;
		*/

	for (map<word, word>::iterator it = a.target.begin(); it != a.target.end(); ++it)
		printf("%x	%x\n", it->first, it->second);
	system("pause");
    return 0;
}

