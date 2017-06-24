#include <unordered_map>
#include <atomic>
#include "thread_safe.h"
#include "hash.h"
#pragma once

class Solver {
public:
	virtual vector<vector<String>> RunBatch(const vector<String> &batch) = 0;
};

class NaiveSerial : public Solver {
public:
	unordered_map<DoubleHashValue, String> table;

	void Insert(const String &pattern);
	void Remove(const String &pattern);
	vector<String> Query(const String &text);
	vector<vector<String>> RunBatch(const vector<String> &batch);
};

class NaiveParallel : public Solver {
public:
	unordered_map<DoubleHashValue, String> table;

	int thread_cnt;
	vector<thread> workers;
	vector<DoubleHashValue> hashs; // no need to use safe_vector as threads write different slots
	safe_vector<pair<int, int>> occurs;
	vector<vector<String>> results;
	atomic<int> task_idx, sub_idx;
	mutex mtx;

	NaiveParallel() = delete;
	NaiveParallel(int thread_cnt) : thread_cnt(thread_cnt) { workers.resize(thread_cnt); }

	void Insert(DoubleHashValue key, const String &pattern);
	void Remove(DoubleHashValue key);
	vector<String> Query(const String &text);
	vector<vector<String>> RunBatch(const vector<String> &batch);

	void HashWorker(const vector<String> &batch);
	void QueryWorker(const String &text, const StringHash &hash);
};