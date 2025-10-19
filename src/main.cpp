#include <iostream>
#include <cstdlib>
#include "../include/manager/InputManager.hpp"
#include "../include/manager/BenchmarkManager.hpp"

void clearConsole() {
    std::system("clear");
}

int main() {
    std::string tmp;
    bool shouldExit = false;

    InputManager inputManager;
    BenchmarkManager benchmarkManager;

    DataStructure* ds = nullptr;
    Algorithm* algo = nullptr;

    BenchmarkManager::BenchmarkResult benchmarkResult;

    std::cout << "RayStruct++: Structures and Algorithms Benchmarking Tool" << std::endl;
    std::cout << "Type 'exit' at any point to quit." << std::endl;

    while (!shouldExit) {
        // delete instances to avoid memory leaks
        if (ds) {
            delete ds;
            ds = nullptr;
        }
        if (algo) {
            delete algo;
            algo = nullptr;
        }

        auto inputResult = inputManager.selectStructureAndAlgorithm();
        shouldExit = inputResult.shouldExit;

        if (shouldExit) break;

        ds = DataStructureFactory::createDataStructure(inputResult.selectedStructure);
        
        if (!ds) {
            std::cout << "\nFailed to create data structure instance." << std::endl;
            continue;
        } else {
            algo = AlgorithmFactory::createAlgorithm(inputResult.selectedAlgorithm);
            if (!algo) {
                std::cout << "\nFailed to create algorithm instance." << std::endl;
                //continue;
            }
        }

        if (ds && algo) {
            shouldExit = inputManager.populateDS(ds);

            if (shouldExit) break;

            if (ds->getElements().size() >= 2) {
                clearConsole();
                benchmarkResult = benchmarkManager.runBenchmark(ds, algo);
            } else {
                std::cout << "\nData structure size is too small. Cannot run benchmark." << std::endl;
                continue;
            }

        } else {
            std::cout << "\nData structure or algorithm instance is null." << std::endl;
            continue;
        }

        std::cout << "\nBenchmark successful" << std::endl;
        std::cout << "\nExecution Time (ms): " << benchmarkResult.executionTimeMs << std::endl;
        std::cout << "\nMemory Usage (B): " << benchmarkResult.memoryUsedBytes << std::endl;
        if (ds && algo && ds->getElements().size() <= 20) {
            std::cout << "\nOperations: " << std::endl;
            algo->executeAndDisplay(ds);
        }
        std::cout << "\n\nPress a key to continue" << std::endl;
        std::cout << ">>> ";
        std::cin >> tmp;
        std::cin.ignore(); // Clear input buffer
        clearConsole();

        if (tmp == "exit") {
            break;
        }
    }

    // delete instances to avoid memory leaks
    if (ds) delete ds;
    if (algo) delete algo;

    return 0;
}
