#include "generator.h"

int main()
{
	Generator gen(R"(..\data\Homer\)", 100, 500);
	gen.BuildPatterns(1000000, uniform_int_distribution<int>(20, 50));
	
	FILE *fout = fopen(R"(..\data\Homer.input)", "w");
	for (int i = 0; i < 100; i++) // batch
		gen.Generate(fout, 10000, 100, 10);
	fclose(fout);
}