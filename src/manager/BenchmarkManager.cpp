// src/manager/BenchmarkManager.cpp
#include "../../include/manager/BenchmarkManager.hpp"
// OS specific includes
#if defined(__linux__)
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#endif

// Captures platform-specific RSS/shared/private memory stats after a run.
void BenchmarkManager::getCurrentRSSBytes() {
// UNIX systems
#if defined(__linux__)
    // Read memory info from /proc/self/statm
    int tSize = 0, resident = 0, share = 0;
    std::ifstream buffer("/proc/self/statm");
    buffer >> tSize >> resident >> share;
    buffer.close();

    // Calculate memory in kB
    const long pageSizeKb = sysconf(_SC_PAGE_SIZE) / 1024;
    const double rssKb = resident * pageSizeKb;
    const double sharedKb = share * pageSizeKb;
    const double privateKb = rssKb - sharedKb;

    std::cout << "\nMemory usage at end:\n";
    std::cout << "[Linux] RSS - " << rssKb << " kB\n";
    std::cout << "[Linux] Shared Memory - " << sharedKb << " kB\n";
    std::cout << "[Linux] Private Memory - " << privateKb << " kB\n";

    lastRSSKb = rssKb;
    lastSharedKb = sharedKb;
    lastPrivateKb = privateKb;

// Windows systems
#elif defined(_WIN32)

    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(),
                             reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc),
                             sizeof(pmc))) {

        const double rssKb = static_cast<double>(pmc.WorkingSetSize) / 1024.0;
        const double privateKb = static_cast<double>(pmc.PrivateUsage) / 1024.0;
        const double sharedKb = rssKb - privateKb;

        std::cout << "[Windows] RSS (Working Set): " << rssKb << " kB\n";
        std::cout << "[Windows] Private Memory:    " << privateKb << " kB\n";
        std::cout << "[Windows] Shared Estimate:   " << sharedKb << " kB";
    }

#endif
}

// Prints the memory snapshot captured before the benchmark started.
void BenchmarkManager::getLastRssBytes() {
    std::cout << "\nMemory usage at start:\n";
    std::cout << "[Linux] RSS - " << lastRSSKb << " kB\n";
    std::cout << "[Linux] Shared Memory - " << lastSharedKb << " kB\n";
    std::cout << "[Linux] Private Memory - " << lastPrivateKb << " kB\n";
}

// Times the algorithm execution and reports both duration and RSS memory use.
void BenchmarkManager::runBenchmark(DataStructure* ds, Algorithm* algo) {
    std::cout << "\nBenchmark Metrics =>" << std::endl;

    getLastRssBytes();
    auto start = std::chrono::high_resolution_clock::now(); // start timing
    algo->execute(ds); // polymorphic call
    auto end = std::chrono::high_resolution_clock::now(); // end timing
    getCurrentRSSBytes();

    // Calculate execution time in milliseconds
    double executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "\nExecution time: " << executionTimeMs << "ms\n";

}
