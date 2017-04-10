#include "generator.h"

int main()
{
	Generator gen(R"(..\data\Homer\)");
	gen.BuildPatterns(100000, uniform_int_distribution<int>(2, 10));
	
	FILE *fout = fopen(R"(..\data\Homer.input)", "w");
	for (int i = 0; i < 100; i++)
		gen.Generate(fout, 10, 1, 1);
	fclose(fout);
}