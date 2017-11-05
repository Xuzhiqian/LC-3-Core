// LC-3 Core.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Memory.h"
#include "ControlUnit.h"
#include "ProcessingUnit.h"
#include "Assembly.h"
#include <iostream>
#include <regex>
#define MAXLINE 0xa000
using namespace std;
word target[MAXLINE];

void GeneOBJ(char *name_c) {
	string name = name_c;
	name = name.substr(0, name.find('.'));
	FILE *output;
	if (fopen_s(&output, (name + ".obj").c_str(), "wb")) {
		printf("\nCreate file error!");
		exit(0);
	}
	fwrite(target, SIZE * 2, 1, output);
	for (int i = 1; i <= target[0]; i++) fwrite(target + i, SIZE, 1, output);
	fclose(output);
}
int main(int argv, char **argc)
{
	if (argv != 3) {
		printf("Invalid format!\n");
		system("pause");
		exit(0);
	}
	FILE *input;
	if (fopen_s(&input, argc[2], "r")) {
		printf("Cannot open file!\n");
		system("pause");
		exit(0);
	}
	Memory m;
	ProcessingUnit p;
	ControlUnit c(&p, &m);
	Assembly a;

	if (strcmp(argc[1], "-hex") == 0) {
		target[0] = 0;
		word value;
		while (!feof(input)) {
			fscanf_s(input, "%x", &value);
			if (!feof(input)) target[++target[0]] = value;
		}
		GeneOBJ(argc[2]);
		printf("OBJ file has been successfully created!\n");
	}
	else if (strcmp(argc[1], "-asm") == 0) {
		target[1] = a.Assemble(input);
		if (target[1] == ERROR) {
			printf("Compile Error!\n");
			system("pause");
			exit(0);
		}
		target[0] = a.target.size() + 1;
		int i = 2;
		for (map<word, word>::iterator it = a.target.begin(); it != a.target.end(); it++)
			target[i++] = it->second;
		GeneOBJ(argc[2]);
		printf("OBJ file has been successfully created!\n");
	}
	else if (strcmp(argc[1], "-run") == 0) {
		word num=0;
		fread(&num, SIZE * 2, 1, input);
		word start=0;
		fread(&start, SIZE, 1, input);
		c.SetPC(start);
		word value=0;
		for (int i = 0; i < num - 1; i++) {
			fread(&value, SIZE, 1, input);
			m.LoadProgramPiece(start + i, value);
		}
		c.Process();
	}
	else {
		printf("\nInvalid format!");
		system("pause");
		exit(0);
	}
	fclose(input);
	system("pause");
	return 0;
}

