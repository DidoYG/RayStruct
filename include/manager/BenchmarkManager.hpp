// include/manager/BenchmarkManager.hpp
#pragma once
#include <chrono>
#include <memory>
#include "../structure/DataStructureFactory.hpp"
#include "../algorithm/AlgorithmFactory.hpp"

class BenchmarkManager {
public:
    BenchmarkManager() = default;

    struct BenchmarkResult {
        double executionTimeMs;
        size_t memoryUsedBytes;
    };

    BenchmarkResult runBenchmark(DataStructure* ds, Algorithm* algo);
};
