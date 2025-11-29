// include/manager/BenchmarkManager.hpp
#pragma once
#include <iostream>
#include <chrono>
#include <memory>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include "../structure/DataStructureFactory.hpp"
#include "../algorithm/AlgorithmFactory.hpp"

class BenchmarkManager {
private:
    // Resource usage tracking
    double lastRSSKb = 0, lastSharedKb = 0, lastPrivateKb = 0;

    // Helper functions for resource tracking
    // Captures the ending RSS/shared/private stats.
    void getCurrentRSSBytes();
    // Prints the previously sampled RSS/shared/private stats.
    void getLastRssBytes();
    
public:
    // Constructor
    BenchmarkManager() = default;

    // Executes the algorithm while recording timing and memory metrics.
    void runBenchmark(DataStructure* ds, Algorithm* algo);
};
