#include <windows.h>
#include "utils.h"
#include "solver.h"
#pragma once

class Evaluator {
public:
	Solver * const solver;

	Evaluator() = delete;
	Evaluator(Solver *solver) : solver(solver) {}
	void Run(const char *input_file, const char *output_file, const char *answer_file);
};