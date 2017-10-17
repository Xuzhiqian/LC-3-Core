// LC-3 Core.cpp : 定义控制台应用程序的入口点。
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

	char s[100];
	gets_s(s, WORD_LENGTH + 1);

	word t = 0;
	for (int i = WORD_LENGTH - 1; i >= 0; i--)
		if (s[i] == '1')
			t += 1 << (WORD_LENGTH-1-i);
	word start = t;
	int index = 0;

	while (s[0] != '#') {
		gets_s(s, WORD_LENGTH + 1);
		if (s[0] != '#') {
			t = 0;
			for (int i = WORD_LENGTH - 1; i >= 0; i--)
				if (s[i] == '1')
					t += 1 << (WORD_LENGTH - 1 - i);
			m.Store(start + index, t);
			index++;
		}
	}
	c.SetPC(start);
	c.Execute();
    return 0;
}

