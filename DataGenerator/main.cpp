#include "generator.h"

int main()
{
	Generator gen(R"(..\data\Homer\)", 100, 500);
	gen.BuildPatterns(100000, uniform_int_distribution<int>(10, 50));
	
	FILE *fout = fopen(R"(..\data\Homer.input)", "w");
	for (int i = 0; i < 50; i++) // batch
		gen.Generate(fout, 10000, 100, 10);
	for (int i = 0; i < 50; i++)
		gen.Generate(fout, 0, 100, 10);
	fclose(fout);
}