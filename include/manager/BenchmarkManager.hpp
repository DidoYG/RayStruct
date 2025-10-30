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
    void getCurrentRSSBytes();
    void getLastRssBytes();
    
public:
    // Constructor
    BenchmarkManager() = default;

    // Run benchmark on given data structure and algorithm
    void runBenchmark(DataStructure* ds, Algorithm* algo);
};
