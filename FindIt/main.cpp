#include "solver.h"
#include "evaluator.h"

int main()
{
	NaiveSerial serial;
	Evaluator evaluator1(&serial);
	evaluator1.Run(R"(..\data\Homer.input)", R"(..\data\Homer.answer)", NULL);

	NaiveParallel parallel(8);
	Evaluator evaluator2(&parallel);
	evaluator2.Run(R"(..\data\Homer.input)", R"(..\data\Homer.output)", R"(..\data\Homer.answer)");

	scanf("\n");
}