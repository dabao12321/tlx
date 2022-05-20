#define DEBUG 0
#define DEBUG_PRINT 0

#include <tlx/container/btree_set_with_pma.hpp>

// #include <tlx/die.hpp>

// #include <cmath>
// #include <cstddef>
// #include <iostream>
// #include <set>
// #include <vector>

#include <random>
#include <vector>
#include <algorithm>
#include <functional>
#include <set>
#include <sys/time.h>
// #include <cilk/cilk.h>
// #include <cilk/cilk_api.h>
#include <parallel.h>
#include "PMA.hpp"

#define PARALLEL_RUNS 0
#define PARALLEL_FIND_SUM 0


template <class T>
std::vector<T> create_random_data(size_t n, size_t max_val,
                                  std::seed_seq &seed) {

  std::mt19937_64 eng(seed); // a source of random data

  std::uniform_int_distribution<T> dist(0, max_val);
  std::vector<T> v(n);

  generate(begin(v), end(v), bind(dist, eng));
  return v;
}

// template <typename T, int leaf_max, int internal_max>
// struct Traits : tlx::btree_default_traits<T, T> {
//     static const bool self_verify = false;
//     static const bool debug = false;
//     static const int leaf_slots = leaf_max;
//     static const int inner_slots = internal_max;
// };

template <class T>
void test_btree_unordered_insert(uint64_t max_size, std::seed_seq &seed, uint64_t* times) {
  if (max_size > std::numeric_limits<T>::max()) {
    max_size = std::numeric_limits<T>::max();
  }
  std::vector<T> data =
      // create_random_data<T>(max_size, std::numeric_limits<T>::max(), seed);
      create_random_data<T>(max_size, 10000, seed);

  // std::set<T> inserted_data;

  uint64_t start, end;

  // save insertion, find, iter sum, naive sum times

  // BTree<T, T> s;

  // Traits<T, leaf_max, internal_max> traits;
  // tlx::btree_set<T, std::less<T>, traits> s;

  tlx::btree_set<T> s;
	// PMA<1024, T, true, T> s;

  start = get_usecs();
  for (uint32_t i = 0; i < max_size; i++) {
#if DEBUG_PRINT
    printf("inserting data[%u] = %lu\n", i, data[i]);
#endif
		// s.insert({data[i], data[i]});
		s.insert(data[i]);

#if DEBUG
		bool passed = true;
		for(uint32_t j = 0; j <= i; j++) {
			// if (!s.has(data[j])) {
			if (!s.exists(data[j])) {
				printf("missing data[%u] = %lu\n", j, data[j]);
				passed = false;
				// assert(false);
			}
			// assert(s.exists(data[j]));
		}

		assert(passed);
#endif

  }
  end = get_usecs();

  times[0] = end - start;
  printf("\ninsertion,\t %lu,", end - start);

	printf("\n");
  // SERIAL FIND
  start = get_usecs();
  for (uint32_t i = 1; i < max_size; i++) {
    if (!s.exists(data[i])) {
			printf("missing elt %lu at idx %u\n", data[i], i);
			assert(false);
		}
		// assert(s.exists(data[i]));
    // if (node == nullptr) {
    //   printf("\ncouldn't find data in btree at index %u\n", i);
    //   exit(0);
    // }
  }
  end = get_usecs();
  printf("\nfind all,\t %lu,\n", end - start);
	/*
#if TIME_INSERT
  // start = get_usecs();
  // for (int i = 0; i < max_size * 10; i++ ) {
  //   end = get_usecs();
  // }
  // end = get_usecs();

  // printf("\ntime to time %lu", end - start);

  printf("\n\t insert_locate_time: \t %lu", s.get_insert_locate_time());
  printf("\n\t insert_insert_time: \t %lu", s.get_insert_insert_time());
  printf("\n\t insert_promote_time: \t %lu", s.get_insert_promote_time());
  return;
#endif
*/
  // PARALLEL FIND 20 MIL
  std::seed_seq seed2{1};

	// generate n / 10 random elts
  std::vector<T> data_to_search =
      create_random_data<T>(max_size / 10, std::numeric_limits<T>::max(), seed2);

	// pick n/10 from the input
	for(uint32_t i = 0; i < max_size; i+=10) {
		if (i < max_size) { data_to_search.push_back(data[i]); }
	}

	// shuffle them
  std::mt19937_64 g(seed); // a source of random data
	std::shuffle(data_to_search.begin(), data_to_search.end(), g);

	std::vector<T> partial_sums(getWorkers() * 8);

  start = get_usecs();
  parallel_for (uint32_t i = 0; i < data_to_search.size(); i++) {
		partial_sums[getWorkerNum() * 8] += s.exists(data_to_search[i]);
  }

  end = get_usecs();

	uint64_t parallel_find_time = end - start;

	// sum up results
	T result = 0;
	for(int i = 0; i < getWorkers(); i++) {
		result += partial_sums[i * 8];
	}



  printf("\nparallel find,\t %lu,\tnum found %lu\n", parallel_find_time, result);

  // SERIAL SUM WITH ITERATOR
  uint64_t correct_sum = 0;
  for (uint32_t i = 1; i < max_size; i++) {
    correct_sum += data[i];
  }
	printf("correct sum == %lu\n", correct_sum);

	std::sort(data.begin(), data.end());
	for(int i = 0; i < data.size(); i++) {
		printf("sorted data[%d] = %lu\n", i, data[i]);
	}
  start = get_usecs();
  uint64_t sum = 0;
  auto it = s.begin();
  while (it != s.end()) {
    T el = *it;
    sum += el;
#if DEBUG_PRINT
		printf("\tadvance iterator, elt %lu\n", el);
#endif
    ++it;
  }
  
  end = get_usecs();
  times[2] = end - start;
	if(sum != correct_sum) {
		printf("got sum %lu, should be %lu\n", sum, correct_sum);
		assert(sum == correct_sum);
	}
  printf("\nsum_time with iterator, \t%lu, \tsum_total, \t%lu, \t", end - start,
         sum);

// #endif
}

int main(int argc, char** argv) {
  // printf("B tree node internal size %zu\n", sizeof(tlx::InnerNode));
  // printf("B tree node leaf size %zu\n", sizeof(BTreeNodeLeaf<uint64_t,uint64_t>));
  // test_btree_ordered_insert<uint32_t>(129);
  
  std::seed_seq seed{0};
  // printf("------- ORDERED INSERT --------\n");
  // test_btree_ordered_insert<uint64_t>(100000000);
  printf("------- UNORDERED INSERT --------\n");
  uint64_t times[4];

	int n = atoi(argv[1]);
  // SINGLE RUN
  // test_btree_unordered_insert<uint64_t>(1000000, seed, times);
  test_btree_unordered_insert<uint64_t>(n, seed, times);
	// printf("\ninsert time %lu, find time %lu, sumiter time %lu, sum time %lu\n", times[0], times[1], times[2], times[3]);
	printf("\n");

// #endif

	return 0;
}

