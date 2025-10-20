// src/manager/BenchmarkManager.cpp
#include "../../include/manager/BenchmarkManager.hpp"
#include <cstdio>
#if defined(__linux__)
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#endif

BenchmarkManager::BenchmarkResult BenchmarkManager::runBenchmark(DataStructure* ds, Algorithm* algo) {
    // 1. Benchmark execution time and memory usage
    const auto rssBefore = getCurrentRSSBytes();
    auto start = std::chrono::high_resolution_clock::now();
    algo->execute(ds); // polymorphic call
    auto end = std::chrono::high_resolution_clock::now();
    const auto rssAfter = getCurrentRSSBytes();

    BenchmarkResult result;
    result.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();

    // 2. Measure resident set size delta
    if (rssAfter >= rssBefore) {
        result.memoryUsedBytes = rssAfter - rssBefore;
    } else {
        result.memoryUsedBytes = 0;
    }

    return result;
}

std::size_t BenchmarkManager::getCurrentRSSBytes() {
#if defined(__linux__)
    FILE* fp = std::fopen("/proc/self/statm", "r");
    if (!fp) {
        return 0;
    }

    long residentPages = 0;
    if (std::fscanf(fp, "%*s %ld", &residentPages) != 1) {
        std::fclose(fp);
        return 0;
    }
    std::fclose(fp);

    long pageSize = sysconf(_SC_PAGESIZE);
    if (pageSize <= 0 || residentPages < 0) {
        return 0;
    }

    return static_cast<std::size_t>(residentPages) * static_cast<std::size_t>(pageSize);
#elif defined(_WIN32)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return static_cast<std::size_t>(pmc.WorkingSetSize);
    }
    return 0;
#else
    return 0;
#endif
}
