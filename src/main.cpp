// src/main.cpp
#include <cstdlib>
#include "../include/manager/InputManager.hpp"
#include "../include/manager/BenchmarkManager.hpp"
#include "../include/structure/GraphStructure.hpp"
#include "../include/algorithm/AStar.hpp"
#include "../include/algorithm/Prims.hpp"
#include "../include/algorithm/AlgorithmFactory.hpp"

// Function to clear the console screen
void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

// Entry point of the application
int main() {
    bool shouldExit = false;

    InputManager inputManager;
    BenchmarkManager benchmarkManager;

    DataStructure* ds = nullptr;
    Algorithm* algo = nullptr;

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

            algo = inputManager.createAlgorithm(algorithmSelection);
            if (!algo) {
                std::cout << "\nFailed to create algorithm instance." << std::endl;
                continue;
            }
            
            // Handle algorithm-specific parameters
            if (algorithmSelection.selectedAlgorithm == AlgorithmEnum::HEAP_BUILD) {
                std::string heapType;

                while (true) {
                    std::cout << "\nSelect heap type (min/max)" << std::endl;
                    std::cout << ">>> ";
                    std::cin >> heapType;
                    // Ignore remaining input to avoid issues
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
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
                    // Ignore remaining input to avoid issues
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
            } else if (algorithmSelection.selectedAlgorithm == AlgorithmEnum::A_STAR) {
                auto* graph = dynamic_cast<GraphStructure*>(ds);
                auto* astarAlgo = dynamic_cast<AStar*>(algo);

                if (!graph || !astarAlgo) {
                    std::cout << "\nGraph structure or A* algorithm is unavailable." << std::endl;
                    continue;
                }

                if (graph->getAdjacency().empty()) {
                    std::cout << "\nGraph has no edges. Please add edges before running A*." << std::endl;
                    continue;
                }

                std::string vertexInput;
                int startVertex = -1;
                int goalVertex = -1;

                while (true) {
                    std::cout << "\nEnter start vertex id for A* ('exit' to quit)" << std::endl;
                    std::cout << ">>> ";
                    std::cin >> vertexInput;
                    // Ignore remaining input to avoid issues
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    if (vertexInput == "exit") {
                        shouldExit = true;
                        break;
                    }

                    try {
                        startVertex = std::stoi(vertexInput);
                    } catch (const std::exception&) {
                        std::cout << "\nInvalid vertex id. Please enter an integer value." << std::endl;
                        continue;
                    }

                    if (!graph->hasVertex(startVertex)) {
                        std::cout << "\nVertex not found in the graph. Try again." << std::endl;
                        continue;
                    }

                    astarAlgo->setStart(startVertex);
                    break;
                }

                if (shouldExit) {
                    break;
                }

                while (true) {
                    std::cout << "\nEnter goal vertex id for A* ('exit' to quit)" << std::endl;
                    std::cout << ">>> ";
                    std::cin >> vertexInput;
                    // Ignore remaining input to avoid issues
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    if (vertexInput == "exit") {
                        shouldExit = true;
                        break;
                    }

                    try {
                        goalVertex = std::stoi(vertexInput);
                    } catch (const std::exception&) {
                        std::cout << "\nInvalid vertex id. Please enter an integer value." << std::endl;
                        continue;
                    }

                    if (!graph->hasVertex(goalVertex)) {
                        std::cout << "\nVertex not found in the graph. Try again." << std::endl;
                        continue;
                    }

                    astarAlgo->setGoal(goalVertex);
                    break;
                }

                if (shouldExit) {
                    break;
                }
            } else if (algorithmSelection.selectedAlgorithm == AlgorithmEnum::PRIMS) {
                auto* graph = dynamic_cast<GraphStructure*>(ds);
                auto* primsAlgo = dynamic_cast<Prims*>(algo);

                if (!graph || !primsAlgo) {
                    std::cout << "\nGraph structure or Prim's algorithm is unavailable." << std::endl;
                    continue;
                }

                if (graph->getAdjacency().empty()) {
                    std::cout << "\nGraph has no edges. Please add edges before running Prim's algorithm." << std::endl;
                    continue;
                }

                std::string vertexInput;
                int startVertex = -1;

                while (true) {
                    std::cout << "\nEnter starting vertex id for Prim's (or type 'auto' to auto-select, 'exit' to quit)" << std::endl;
                    std::cout << ">>> ";
                    std::cin >> vertexInput;
                    // Ignore remaining input to avoid issues
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    if (vertexInput == "exit") {
                        shouldExit = true;
                        break;
                    }

                    std::string lowered = vertexInput;
                    std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

                    if (lowered == "auto" || lowered == "default" || lowered == "skip") {
                        startVertex = -1;
                        break;
                    }

                    try {
                        startVertex = std::stoi(vertexInput);
                    } catch (const std::exception&) {
                        std::cout << "\nInvalid vertex id. Please enter an integer value." << std::endl;
                        continue;
                    }

                    if (!graph->hasVertex(startVertex)) {
                        std::cout << "\nVertex not found in the graph. Try again." << std::endl;
                        continue;
                    }

                    break;
                }

                if (shouldExit) {
                    break;
                }

                primsAlgo->setStart(startVertex);
            }

            clearConsole();
            // Run benchmark
            benchmarkManager.runBenchmark(ds, algo);

            // Display operations/results if applicable
            if (algorithmSelection.selectedAlgorithm != AlgorithmEnum::HEAP_SELECTION && algorithmSelection.selectedAlgorithm != AlgorithmEnum::A_STAR && algorithmSelection.selectedAlgorithm != AlgorithmEnum::PRIMS) {
                if (ds && algo && ds->getElements().size() <= 20) {
                    std::cout << "\nOperations: " << std::endl;
                    algo->executeAndDisplay(ds);
                }
            } else {
                if (ds && algo) {
                    std::cout << "\nResult: " << std::endl;
                    algo->executeAndDisplay(ds);
                }
            }

            std::string input;
            std::cout << "\nRun another algorithm on the same data structure? (y/n)" << std::endl;
            std::cout << ">>> ";
            std::cin >> std::ws;
            std::getline(std::cin, input);

            if (input == "exit") {
                shouldExit = true;
                break;
            }

            // Convert input to lowercase for comparison
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

    // Cleanup any loaded custom algorithm libraries
    AlgorithmFactory::cleanupCustomLibraries();

    return 0;
}
