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
    double lastRSSKb = 0, lastSharedKb = 0, lastPrivateKb = 0;

    void getCurrentRSSBytes();
    void getLastRssBytes();
    
public:
    BenchmarkManager() = default;

    void runBenchmark(DataStructure* ds, Algorithm* algo);
};
