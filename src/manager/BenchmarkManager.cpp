// src/manager/BenchmarkManager.cpp
#include "../../include/manager/BenchmarkManager.hpp"

BenchmarkManager::BenchmarkResult BenchmarkManager::runBenchmark(DataStructure* ds, Algorithm* algo) {
    // 1. Benchmark execution time
    auto start = std::chrono::high_resolution_clock::now();
    algo->execute(ds); // polymorphic call
    auto end = std::chrono::high_resolution_clock::now();

    BenchmarkResult result;
    result.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();

    // 2. Optionally measure memory usage
    // You could approximate by checking container sizes (vector::capacity, etc.)
    result.memoryUsedBytes = ds->getElements().capacity() * sizeof(int);

    return result;
}