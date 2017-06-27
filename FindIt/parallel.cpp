#include <algorithm>
#include "solver.h"

#pragma region Parallel

const int Parallel::chunk_size = 16;

vector<vector<String>> Parallel::RunBatch(const vector<String> &batch)
{
	vector<vector<String>> results;

	batch_id++;
	task_idx = 0;
	queries.clear();
	for (int i = 0; i < thread_cnt; i++)
		workers[i] = thread(&Parallel::HashWorker, this, batch);
	for (int i = 0; i < thread_cnt; i++)
		workers[i].join();

	task_idx = 0;
	sub_idx = new atomic<size_t>[queries.size()];
	occurs.clear();
	occurs.resize(queries.size());
	for (int i = 0; i < thread_cnt; i++)
		workers[i] = thread(&Parallel::QueryWorker, this);
	for (int i = 0; i < thread_cnt; i++)
		workers[i].join();
	for (size_t i = 0; i < queries.size(); i++) {
		String text = batch[queries[i].second.line_id];
		sort(occurs[i].begin(), occurs[i].end());
		vector<String> result;
		for (size_t j = 0; j < occurs[i].size(); j++)
			result.push_back(String(text.s + occurs[i][j].first, occurs[i][j].second));
		results.push_back(result);
	}
	delete[] sub_idx;
	
	return results;
}

void Parallel::HashWorker(const vector<String> &batch)
{
	size_t i;
	while ((i = task_idx.fetch_add(1)) < batch.size()) {
		GramHash hash = GramHash(batch[i] + 1);
		Timestamp ts = Timestamp(batch[i].s[0] == 'A', batch_id, i);
		auto a = table[hash.first_hash];
		auto b = a[hash.key_gram_count];
		auto c = b[hash.key_hash];
		auto d = c[hash.full_length];
		auto e = d[hash.full_hash];
		switch (batch[i].s[0]) {
		case 'A': case 'D':
			e.push_back(ts);
				break;
		case 'Q': queries.push_back(make_pair(hash, ts)); break;
		}
	}
}

void Parallel::QueryWorker()
{
	size_t i, j;
	while ((i = task_idx.fetch_add(1)) < queries.size()) {
		pair<GramHash, Timestamp> q = queries[i];
		GramHash hash = q.first;
		Timestamp ts = q.second;
		while ((j = sub_idx[i].fetch_add(chunk_size)) < hash.gram_count) {
			for (int jj = j; jj < jj + chunk_size && jj < hash.gram_count; jj++) {
				DoubleHashValue first_expected = hash(jj, jj + 1);
				if (table.find(first_expected) == table.end())
					continue;
				auto key_gram_table = table[first_expected];
				for (auto p = key_gram_table.begin(); p != key_gram_table.end(); p++) {
					int key_gram_count = p->first;
					int key_expected = hash(jj, jj + key_gram_count);
					auto key_table = p->second;
					if (key_table.find(key_expected) == key_table.end())
						continue;
					auto full_length_table = key_table[key_expected];
					for (auto q = full_length_table.begin(); q != full_length_table.end(); q++) {
						int full_length = q->first;
						int full_expected = ((StringHash)hash)(jj, jj + full_length);
						auto full_table = q->second;
						if (full_table.find(full_expected) == full_table.end())
							continue;
						vector<Timestamp> tslist = full_table[full_expected];
						size_t k;
						for (k = 0; k < tslist.size(); k++)
							if (ts < tslist[k])
								break;
						if (tslist[--k].exist)
							occurs[k].push_back(make_pair(jj, jj + full_length));
					}
				}
			}
		}
	}
}

#pragma endregion