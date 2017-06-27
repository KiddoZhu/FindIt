#include "generator.h"

int main()
{
	Generator gen(R"(..\data\Homer\)", 100, 500);
	gen.BuildPatterns(1000000, uniform_int_distribution<int>(10, 30));
	
	FILE *fout = fopen(R"(..\data\Homer.input)", "w");
	for (int i = 0; i < 2; i++) // batch
		gen.Generate(fout, 1000, 10, 10);
	fclose(fout);
}