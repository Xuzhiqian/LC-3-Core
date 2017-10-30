// LC-3 Core.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Memory.h"
#include "ControlUnit.h"
#include "ProcessingUnit.h"
#include "Assembly.h"
#include <iostream>
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
	a.Filter(ass);
	fclose(ass);


	system("pause");
    return 0;
}

