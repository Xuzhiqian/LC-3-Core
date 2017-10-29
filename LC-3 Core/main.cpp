// LC-3 Core.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Memory.h"
#include "ControlUnit.h"
#include "ProcessingUnit.h"

int main()
{
	Memory m;
	ProcessingUnit p;
	ControlUnit c(&p,&m);

	
	word v, start;

	scanf_s("%x", &start);
	c.SetPC(start);

	while (scanf_s("%x",&v))
		m.Store(start++,v);
	
	c.Process();
	system("pause");
    return 0;
}

