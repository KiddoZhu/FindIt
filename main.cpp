#include "solver.h"
#include "evaluator.h"

int main()
{
	Evaluator<NaiveSerial> evaluator("test_input.txt");

	evaluator.Run();
}