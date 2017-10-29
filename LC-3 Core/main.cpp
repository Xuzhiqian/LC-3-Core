// LC-3 Core.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Memory.h"
#include "ControlUnit.h"
#include "ProcessingUnit.h"
#include "Assembly.h"

int main()
{
	Memory m;
	ProcessingUnit p;
	ControlUnit c(&p,&m);
	Assembly a;

	
	word v, start;
	/*
	scanf_s("%x", &start);
	c.SetPC(start);

	while (scanf_s("%x",&v))
		m.Store(start++,v);
	
	c.Process();
	*/

	FILE *ass = fopen("1.asm", "r");
	FILE *out = fopen("result.txt", "w");
	if (ass != NULL)
	{
		a.Filter(ass);
		fputs(a.buffer,out);
	}
	else printf("error!\n");
	fclose(ass);
	fclose(out);


    return 0;
}

