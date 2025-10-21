// src/manager/BenchmarkManager.cpp
#include "../../include/manager/BenchmarkManager.hpp"
#if defined(__linux__)
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#endif

void BenchmarkManager::getCurrentRSSBytes() {
#if defined(__linux__)

    int tSize = 0, resident = 0, share = 0;
    std::ifstream buffer("/proc/self/statm");
    buffer >> tSize >> resident >> share;
    buffer.close();

    const long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;
    const double rss_kb = resident * page_size_kb;
    const double shared_kb = share * page_size_kb;
    const double private_kb = rss_kb - shared_kb;

    std::cout << "[Linux] RSS - " << rss_kb << " kB\n";
    std::cout << "[Linux] Shared Memory - " << shared_kb << " kB\n";
    std::cout << "[Linux] Private Memory - " << private_kb << " kB\n";

#elif defined(_WIN32)

    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(),
                             reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc),
                             sizeof(pmc))) {

        const double rss_kb = static_cast<double>(pmc.WorkingSetSize) / 1024.0;
        const double private_kb = static_cast<double>(pmc.PrivateUsage) / 1024.0;
        const double shared_kb = rss_kb - private_kb;

        std::cout << "[Windows] RSS (Working Set): " << rss_kb << " kB\n";
        std::cout << "[Windows] Private Memory:    " << private_kb << " kB\n";
        std::cout << "[Windows] Shared Estimate:   " << shared_kb << " kB";
    }

#endif
}

void BenchmarkManager::runBenchmark(DataStructure* ds, Algorithm* algo) {
    std::cout << "\nBenchmark Metrics =>" << std::endl;

    std::cout << "\nMemory usage at start:\n";
    getCurrentRSSBytes();

    auto start = std::chrono::high_resolution_clock::now();
    algo->execute(ds); // polymorphic call
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "\nMemory usage at end:\n";
    getCurrentRSSBytes();

    double executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "\nExecution time: " << executionTimeMs << "ms\n";

}
