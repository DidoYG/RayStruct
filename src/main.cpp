#include <iostream>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <cctype>
#include "../include/manager/InputManager.hpp"
#include "../include/manager/BenchmarkManager.hpp"

void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

int main() {
    bool shouldExit = false;

    InputManager inputManager;
    BenchmarkManager benchmarkManager;

    DataStructure* ds = nullptr;
    Algorithm* algo = nullptr;

    BenchmarkManager::BenchmarkResult benchmarkResult;

    std::cout << "RayStruct++: Structures and Algorithms Benchmarking Tool" << std::endl;
    std::cout << "Type 'exit' at any point to quit." << std::endl;

    while (!shouldExit) {
        if (ds) {
            delete ds;
            ds = nullptr;
        }
        if (algo) {
            delete algo;
            algo = nullptr;
        }

        auto structureSelection = inputManager.selectStructure();
        shouldExit = structureSelection.shouldExit;
        if (shouldExit) break;

        if (structureSelection.selectedStructure == DataStructureEnum::UNKNOWN) {
            std::cout << "\nNo data structure selected. Restarting selection." << std::endl;
            continue;
        }

        ds = inputManager.createDataStructure(structureSelection.selectedStructure);
        if (!ds) {
            std::cout << "\nFailed to create data structure instance." << std::endl;
            continue;
        }

        shouldExit = inputManager.populateDS(ds, structureSelection.selectedStructure);
        if (shouldExit) break;

        if (ds->getElements().size() < 2) {
            std::cout << "\nData structure size is too small. Cannot run benchmark." << std::endl;
            continue;
        }

        bool reuseStructure = true;

        while (reuseStructure && !shouldExit) {
            if (algo) {
                delete algo;
                algo = nullptr;
            }

            auto algorithmSelection = inputManager.selectAlgorithm(structureSelection.selectedStructure);
            shouldExit = algorithmSelection.shouldExit;
            if (shouldExit) break;

            if (algorithmSelection.selectedAlgorithm == AlgorithmEnum::UNKNOWN) {
                std::cout << "\nNo algorithm selected. Restarting algorithm selection." << std::endl;
                continue;
            }

            algo = inputManager.createAlgorithm(algorithmSelection.selectedAlgorithm);
            if (!algo) {
                std::cout << "\nFailed to create algorithm instance." << std::endl;
                continue;
            }

            if (algorithmSelection.selectedAlgorithm == AlgorithmEnum::HEAP_BUILD) {
                std::string heapType;

                while (true) {
                    std::cout << "\nSelect heap type (min/max)" << std::endl;
                    std::cout << ">>> ";
                    std::cin >> heapType;
                    
                    HeapBuild* heapBuild = dynamic_cast<HeapBuild*>(algo);

                    if (heapType == "min") {
                        heapBuild->setHeapType(true);
                        break;
                    } else if (heapType == "max") {
                        heapBuild->setHeapType(false);
                        break;
                    } else {
                        std::cout << "\nInvalid option. Please enter 'min' or 'max'.\n";
                    }
                }
            } else if (algorithmSelection.selectedAlgorithm == AlgorithmEnum::HEAP_SELECTION) {
                std::string choice;
                int k;

                while (true) {
                    std::cout << "\nFind k-th (smallest/largest)" << std::endl;
                    std::cout << ">>> ";
                    std::cin >> choice;

                    std::cout << "\nEnter k" << std::endl;
                    std::cout << ">>> ";
                    std::cin >> k;

                    HeapSelection* heapSelect = dynamic_cast<HeapSelection*>(algo);

                    if (k <= 0 || k > (int)ds->getElements().size()) {
                        std::cout << "Invalid k: must be between 1 and " << ds->getElements().size() << std::endl;
                        continue;
                    }

                    heapSelect->setK(k);
                    
                    if (choice == "smallest") {
                        heapSelect->setIsSmallest(true);
                        break;
                    } else if (choice == "largest") {
                        heapSelect->setIsSmallest(false);
                        break;
                    } else {
                        std::cout << "\nInvalid option. Please enter 'smallest' or 'largest'.\n";
                    }
                }
            }

            clearConsole();
            benchmarkResult = benchmarkManager.runBenchmark(ds, algo);

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nBenchmark successful" << std::endl;
            std::cout << "\nExecution Time (ms): " << benchmarkResult.executionTimeMs << std::endl;
            std::cout << "\nMemory Usage (B): " << benchmarkResult.memoryUsedBytes << std::endl;
            if (ds && algo && ds->getElements().size() <= 20) {
                std::cout << "\nOperations: " << std::endl;
                algo->executeAndDisplay(ds);
            }

            std::string input;
            std::cout << "\nRun another algorithm on the same data structure? (y/n)" << std::endl;
            std::cout << ">>> ";
            std::getline(std::cin, input);

            if (input == "exit") {
                shouldExit = true;
                break;
            }

            std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

            if (input == "y" || input == "yes") {
                delete algo;
                algo = nullptr;
                clearConsole();
                continue;
            }

            reuseStructure = false;
            delete algo;
            algo = nullptr;
            clearConsole();
        }

        if (shouldExit) break;
    }

    // delete instances to avoid memory leaks
    if (ds) delete ds;
    if (algo) delete algo;

    return 0;
}
