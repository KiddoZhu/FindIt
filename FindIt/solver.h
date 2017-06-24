#include <unordered_map>
#include "thread_safe.h"
#include "hash.h"
#pragma once

class Solver {
public:
	virtual vector<vector<String>> RunBatch(vector<String> batch) = 0;
};

class NaiveSerial : public Solver {
public:
	unordered_map<DoubleHashValue, String> table;

	void Insert(String pattern);
	void Remove(String pattern);
	vector<String> Query(String text);
	vector<vector<String>> RunBatch(vector<String> batch);
};

class NaiveParallel : public Solver {
public:
	int thread_cnt = 8;
	safe_unordered_map<DoubleHashValue, String> table;

	void Insert(String pattern);
	void Remove(String pattern);
	vector<String> Query(String text);
	vector<vector<String>> RunBatch(vector<String> batch);
};