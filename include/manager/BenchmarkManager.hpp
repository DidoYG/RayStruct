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
    void getCurrentRSSBytes();
    
public:
    BenchmarkManager() = default;

    void runBenchmark(DataStructure* ds, Algorithm* algo);
};
