#include "solver.h"
#include "evaluator.h"

int main()
{
	NaiveSerial solver;
	Evaluator evaluator = Evaluator(&solver);

	evaluator.Run(R"(..\data\tiny.input)", R"(..\data\tiny.output)", NULL);
}