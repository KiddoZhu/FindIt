#include <windows.h>
#include "utils.h"
#include "solver.h"

template <class SolverType>
class Evaluator {
public:
	const char *dataset;
	SolverType solver;

	unsigned char *input;
	UInt64 length;
	LARGE_INTEGER cycles, frequency;

	Evaluator() = delete;
	Evaluator(const char *filename);
	~Evaluator();
	void Run();
};

template <class SolverType>
Evaluator<SolverType>::Evaluator(const char *filename) : dataset(filename)
{
	HANDLE file, mapping;

	if ((file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))
		 == INVALID_HANDLE_VALUE)
		throw("File cannot be opened.");
	length.part[0] = GetFileSize(file, &length.part[1]);
	if ((mapping = CreateFileMapping(file, NULL, PAGE_READONLY, length.part[1], length.part[0], NULL))
		== NULL)
		throw("File mapping cannot be opend.");
	if ((input = (unsigned char *) MapViewOfFile(mapping, FILE_MAP_COPY, 0, 0, 0)) // Copy-On-Write
		== NULL)
		throw("Map view of file cannot be established.");
}

template <class SolverType>
Evaluator<SolverType>::~Evaluator()
{
	UnmapViewOfFile(input);
}

template <class SolverType>
void Evaluator<SolverType>::Run()
{
	vector<String> batch;
	unsigned char *begin, *end;

	QueryPerformanceFrequency(&frequency);
	cycles.QuadPart = 0;
	// Assume line is in Windows style (aka "\r\n")
	for (begin = input; begin < input + length.all; begin = end + 1) {
		for (end = begin; *end != '\n'; end++);
		end[-1] = ' ';
		if (*begin == 'E') {
			LARGE_INTEGER start, stop;

			QueryPerformanceCounter(&start);
			solver.RunBatch(batch);
			QueryPerformanceCounter(&stop);
			cycles.QuadPart += stop.QuadPart - start.QuadPart;

			batch.clear();
		}
		else
			batch.push_back(String(begin, end - begin));
	}

	printf("Dataset: %s, Model: %s, Time elapsed: %.3f s\n",
		dataset, typeid(SolverType).name(), (float) cycles.QuadPart / frequency.QuadPart);
}