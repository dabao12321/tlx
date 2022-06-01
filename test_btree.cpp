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
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <parallel.h>

#include <tlx/container/btree_map.hpp>
#include <tlx/container/btree_multimap.hpp>
#include <tlx/container/btree_multiset.hpp>
#include <tlx/container/btree_set.hpp>
#include <tlx/container/btree.hpp>


static long get_usecs() {
    struct timeval st;
    gettimeofday(&st,NULL);
    return st.tv_sec*1000000 + st.tv_usec;
}

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
      create_random_data<T>(max_size, std::numeric_limits<T>::max(), seed);
      //create_random_data<T>(max_size, 100, seed);

  // std::set<T> inserted_data;

  uint64_t start, end;

  // save insertion, find, iter sum, naive sum times

  // BTree<T, T> s;

  // Traits<T, leaf_max, internal_max> traits;
  // tlx::btree_set<T, std::less<T>, traits> s;

  tlx::btree_set<T> s;

  start = get_usecs();
  for (uint32_t i = 0; i < max_size; i++) {
    s.insert(data[i]);
  }
  end = get_usecs();

  times[0] = end - start;
  printf("\ninsertion,\t %lu,", end - start);

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

#if TIMING_EXPERIMENTS
  // SERIAL FIND
  start = get_usecs();
  for (uint32_t i = 0; i < max_size; i++) {
	  if (!s.exists(data[i])) {
			printf("missing elt %lu at idx %u\n", data[i], i);
			assert(false);
		}
  }
  end = get_usecs();

  times[1] = end - start;
  printf("\nfind all,\t %lu,", end - start);

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
#if DEBUG
	uint64_t correct_sum = 0;
  for (uint32_t i = 0; i < max_size; i++) {
    correct_sum += data[i];
  }
#endif

  start = get_usecs();
  uint64_t sum = 0;
  auto it = s.begin();
  while (it != s.end()) {
    T el = *it;
    sum += el;
    ++it;
  }
  
  end = get_usecs();
  times[2] = end - start;
#if DEBUG
  printf("\ncorrect sum total: \t %lu", correct_sum);
#endif
  printf("\nsum_time with iterator, \t%lu, \tsum_total, \t%lu, \t", end - start,
         sum);

// #endif
	// parallel sum
	start = get_usecs();
  uint64_t parallel_sum = s.psum();
  end = get_usecs();
	uint64_t psum_time = end - start;
#if DEBUG
	if(parallel_sum != correct_sum) {
		printf("got parallel sum %lu, should be %lu\n", parallel_sum, correct_sum);
		assert(parallel_sum == correct_sum);
	}
  printf("\npsum_time, \t%lu, \tsum_total, \t%lu, \tcorrect_sum, \t %lu\n", end - start,
         parallel_sum, correct_sum);
#endif
assert(parallel_sum == sum);
printf("\npsum_time, \t%lu, \tsum_total, \t%lu\n", end - start,
          parallel_sum);
#endif

	uint64_t size = s.get_size();
	printf("size in bytes = %lu\n", size);
 	double leaf_density = s.get_leaf_density();
	printf("avg density = %f\n", leaf_density);

}

template <class T>
void test_btree_parallel_unordered_insert(uint64_t max_size, std::seed_seq &seed, int num_copies) {
  if (max_size > std::numeric_limits<T>::max()) {
    max_size = std::numeric_limits<T>::max();
  }
  std::vector<T> data =
      create_random_data<T>(max_size, std::numeric_limits<T>::max(), seed);

  uint64_t start, end;

	std::vector<tlx::btree_set<T>> trees(num_copies);

  start = get_usecs();
	parallel_for(uint32_t j = 0; j < num_copies; j++) {
		for (uint32_t i = 0; i < max_size; i++) {
			trees[j].insert(data[i]);
		}
	}
  end = get_usecs();

  uint64_t insert_time = end - start;
  printf("\ninsertion with %d copies at %lu elts,\t %lu,", num_copies, max_size, insert_time);
	printf("\n");
  
	start = get_usecs();
	parallel_for(uint32_t j = 0; j < num_copies; j++) {
		trees[j].psum();
	}
  end = get_usecs();
	printf("sum time = %lu\n", end - start);

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
  // test_btree_unordered_insert<uint64_t>(n, seed, times);
	// printf("\ninsert time %lu, find time %lu, sumiter time %lu, sum time %lu\n", times[0], times[1], times[2], times[3]);
	test_btree_parallel_unordered_insert<uint64_t>(n, seed, 16);
	printf("\n");

// #endif

	return 0;
}

