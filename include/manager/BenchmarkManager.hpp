// include/manager/BenchmarkManager.hpp
#pragma once
#include <chrono>
#include <memory>
#include <cstddef>
#include "../structure/DataStructureFactory.hpp"
#include "../algorithm/AlgorithmFactory.hpp"

class BenchmarkManager {
private:
    static std::size_t getCurrentRSSBytes();
    
public:
    BenchmarkManager() = default;

    struct BenchmarkResult {
        double executionTimeMs;
        size_t memoryUsedBytes;
    };

    BenchmarkResult runBenchmark(DataStructure* ds, Algorithm* algo);
};
