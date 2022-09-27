#define DEBUG 0
#define DEBUG_PRINT 0

#include <sys/time.h>

#include <algorithm>
#include <fstream>
#include <functional>
#include <random>
#include <set>
#include <vector>
// #include <cilk/cilk.h>
// #include <cilk/cilk_api.h>
#include <parallel.h>

#include <tlx/container/btree_set_with_pma.hpp>

#include "PMA.hpp"
#define PARALLEL_RUNS 0
#define PARALLEL_FIND_SUM 0
#define TIMING_EXPERIMENTS 1

template <class T>
std::vector<T> create_random_data(size_t n, size_t max_val,
                                  std::seed_seq &seed) {
  std::mt19937_64 eng(seed);  // a source of random data

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
std::tuple<bool, uint64_t, uint64_t> test_concurrent_btreeset(
    uint64_t max_size, std::seed_seq &seed) {
  std::vector<T> data =
      create_random_data<T>(max_size, std::numeric_limits<T>::max(), seed);

  uint64_t start, end;

  tlx::btree_set<T> serial_set;

  start = get_usecs();
  for (uint32_t i = 0; i < max_size; i++) {
    serial_set.insert(i);
  }
  end = get_usecs();
  int64_t serial_time = end - start;
  printf("inserted all the data serially in %lu\n", end - start);

  tlx::btree_set<T, std::less<T>, tlx::btree_default_traits<T, T>,
                 std::allocator<T>, true>
      concurrent_set;

  start = get_usecs();
  cilk_for(uint32_t i = 0; i < max_size; i++) {
    concurrent_set.insert(i);
  }

  end = get_usecs();
  uint64_t parallel_time = end - start;
  printf("inserted all the data concurrently in %lu\n", end - start);

  if (serial_set.size() != concurrent_set.size()) {
    printf("the sizes don't match, got %lu, expetected %lu\n",
           concurrent_set.size(), serial_set.size());
    return {false, 0, 0};
  }
  auto it_serial = serial_set.begin();
  auto it_concurrent = concurrent_set.begin();
  bool wrong = false;

  for (uint64_t i = 0; i < serial_set.size(); i++) {
    if (*it_serial != *it_concurrent) {
      printf("don't match in position %lu, got %lu, expected %lu\n", i,
             *it_concurrent, *it_serial);
      wrong = true;
    }
    ++it_serial;
    ++it_concurrent;
  }
  if (wrong) {
    return {false, 0, 0};
  }
  return {true, serial_time, parallel_time};
}

template <class T>
void test_btree_unordered_insert(uint64_t max_size, std::seed_seq &seed,
                                 uint64_t *times) {
  if (max_size > std::numeric_limits<T>::max()) {
    max_size = std::numeric_limits<T>::max();
  }
  std::vector<T> data =
      create_random_data<T>(max_size, std::numeric_limits<T>::max(), seed);

  uint64_t start, end;

  // save insertion, find, iter sum, naive sum times
  tlx::btree_set<T> s;

  start = get_usecs();
  for (uint32_t i = 0; i < max_size; i++) {
#if DEBUG_PRINT
    printf("inserting data[%u] = %lu\n", i, data[i]);
#endif
    s.insert(data[i]);

#if DEBUG
    printf("here\n");
    bool passed = true;
    for (uint32_t j = 0; j <= i; j++) {
      // if (!s.has(data[j])) {
      if (!s.exists(data[j])) {
        printf("missing data[%u] = %lu\n", j, data[j]);
        passed = false;
      }
    }

    assert(passed);
#endif
  }
  end = get_usecs();

  uint64_t insert_time = end - start;
  printf("\ninsertion,\t %lu,", insert_time);
  printf("\n");

  /*
// SERIAL FIND
start = get_usecs();
for (uint32_t i = 1; i < max_size; i++) {
if (!s.exists(data[i])) {
                  printf("missing elt %lu at idx %u\n", data[i], i);
                  assert(false);
          }
}
end = get_usecs();
printf("\nfind all,\t %lu,\n", end - start);
  uint64_t find_all_time = end - start;

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

  */
#if DEBUG
  auto correct_sum_set = std::set<T>();
  // SERIAL SUM WITH ITERATOR
  uint64_t correct_sum = 0;
  for (uint32_t i = 0; i < max_size; i++) {
    correct_sum_set.insert(data[i]);
  }
  for (auto el : correct_sum_set) {
    correct_sum += el;
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
  uint64_t sum_time = end - start;
#if DEBUG
  if (sum != correct_sum) {
    printf("got sum %lu, should be %lu\n", sum, correct_sum);
    assert(sum == correct_sum);
  }
  printf(
      "\nsum_time with iterator, \t%lu, \tsum_total, \t%lu, \tcorrect_sum, \t "
      "%lu\n",
      end - start, sum, correct_sum);
#endif
  printf("\nsum_time with iterator, \t%lu, \tsum_total, \t%lu\n", sum_time,
         sum);

  // parallel sum
  start = get_usecs();
  uint64_t parallel_sum = s.psum();
  end = get_usecs();
  uint64_t psum_time = end - start;
#if DEBUG
  if (parallel_sum != correct_sum) {
    printf("got parallel sum %lu, should be %lu\n", parallel_sum, correct_sum);
    assert(parallel_sum == correct_sum);
  }
  printf("\npsum_time, \t%lu, \tsum_total, \t%lu, \tcorrect_sum, \t %lu\n",
         end - start, parallel_sum, correct_sum);
#endif
  printf("\npsum_time, \t%lu, \tsum_total, \t%lu\n", psum_time, parallel_sum);
  //#endif
  uint64_t size = s.get_size();
  double leaf_density = s.get_leaf_density();
  printf("size in bytes = %lu\n", size);
  printf("avg density = %f\n", leaf_density);

  /*
  #if TIMING_EXPERIMENTS
          std::ofstream outfile;
          outfile.open("pma_times.csv", std::ios_base::app);
          outfile << insert_time << "," << find_all_time << "," <<
  parallel_find_time << "," << sum_time << "," << psum_time << "," << size <<
  "\n"; #endif
  */
}

template <class T>
void test_btree_parallel_unordered_insert(uint64_t max_size,
                                          std::seed_seq &seed, int num_copies) {
  if (max_size > std::numeric_limits<T>::max()) {
    max_size = std::numeric_limits<T>::max();
  }
  std::vector<T> data =
      create_random_data<T>(max_size, std::numeric_limits<T>::max(), seed);

  uint64_t start, end;

  std::vector<tlx::btree_set<T>> trees(num_copies);
  /*
  trees.reserve(num_copies);
  for(int i = 0; i < num_copies; i++) {
          trees.push_back(tlx::btree_set<T>());
  }
  */

  start = get_usecs();
  parallel_for(uint32_t j = 0; j < num_copies; j++) {
    // tlx::btree_set<T> s;
    for (uint32_t i = 0; i < max_size; i++) {
      trees[j].insert(data[i]);
    }
  }
  end = get_usecs();

  uint64_t insert_time = end - start;
  printf("\ninsertion with %d copies at %lu elts,\t %lu,", num_copies, max_size,
         insert_time);
  printf("\n");

  start = get_usecs();
  uint64_t sums[num_copies];
  // for(int i = 0; i < 100; i++) {
  parallel_for(uint32_t j = 0; j < num_copies; j++) {
    // tlx::btree_set<T> s;
    sums[j] = trees[j].psum();
  }
  //	}
  for (uint32_t j = 0; j < num_copies; j++) {
    printf("\ttree %u got parallel sum %lu\n", j, sums[j]);
  }
  end = get_usecs();
  printf("sum time = %lu\n", end - start);

  double leaf_density = trees[0].get_leaf_density();
  printf("avg density = %f\n", leaf_density);
}

int main(int argc, char **argv) {
  // printf("B tree node internal size %zu\n", sizeof(tlx::InnerNode));
  // printf("B tree node leaf size %zu\n",
  // sizeof(BTreeNodeLeaf<uint64_t,uint64_t>));
  // test_btree_ordered_insert<uint32_t>(129);

  std::seed_seq seed{0};
  // printf("------- ORDERED INSERT --------\n");
  // test_btree_ordered_insert<uint64_t>(100000000);
  printf("------- SERIAL UNORDERED INSERT --------\n");
  uint64_t times[4];

  int n = atoi(argv[1]);
  // SINGLE RUN
  // test_btree_unordered_insert<uint64_t>(n, seed, times);

  // test_btree_parallel_unordered_insert<uint64_t>(n, seed, 16);

  printf("\n");

  printf("------- CONCURRENT UNORDERED INSERT --------\n");

  int trials = 1;
  if (argc > 2) {
    trials = atoi(argv[2]);
  }
  // std::seed_seq seed{0};
  // int n = atoi(argv[1]);
  std::vector<uint64_t> serial_times;
  std::vector<uint64_t> parallel_times;
  for (int i = 0; i < trials; i++) {
    auto [correct, serial, parallel] =
        test_concurrent_btreeset<unsigned long>(n, seed);
    if (!correct) {
      printf("got the wrong answer\n");
      return -1;
    }
    serial_times.push_back(serial);
    parallel_times.push_back(parallel);
  }
  std::sort(serial_times.begin(), serial_times.end());
  std::sort(parallel_times.begin(), parallel_times.end());
  printf("%lu, %lu\n", serial_times[trials / 2], parallel_times[trials / 2]);
  return 0;

  // #endif

  return 0;
}
