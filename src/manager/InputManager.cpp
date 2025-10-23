// src/manager/InputManager.cpp
#include "../../include/manager/InputManager.hpp"
#include "../../include/structure/GraphStructure.hpp"

std::string InputManager::trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos)
        return "";
    auto end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

// Helper: convert string to DataStructureEnum
DataStructureEnum InputManager::parseStructure(const std::string& input) {
    std::string s = trim(input);
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    if (s == "list") return DataStructureEnum::LIST;
    if (s == "heap") return DataStructureEnum::HEAP;
    if (s == "graph") return DataStructureEnum::GRAPH;
    if (s == "custom") return DataStructureEnum::CUSTOM;

    return DataStructureEnum::UNKNOWN;
}

// Helper: convert string to AlgorithmEnum
AlgorithmEnum InputManager::parseAlgorithm(const std::string& input) {
    std::string s = trim(input);
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    if (s == "merge sort") return AlgorithmEnum::MERGE_SORT;
    if (s == "insertion sort") return AlgorithmEnum::INSERTION_SORT;
    if (s == "heap build") return AlgorithmEnum::HEAP_BUILD;
    if (s == "heap selection") return AlgorithmEnum::HEAP_SELECTION;
    if (s == "a*" || s == "astar" || s == "a-star") return AlgorithmEnum::A_STAR;
    if (s == "prim" || s == "prims" || s == "prim's") return AlgorithmEnum::PRIMS;
    if (s == "custom") return AlgorithmEnum::CUSTOM;

    return AlgorithmEnum::UNKNOWN;
}

InputManager::StructureSelection InputManager::selectStructure() {
    StructureSelection selection;
    DataStructureEnum structure = DataStructureEnum::UNKNOWN;

    while (structure == DataStructureEnum::UNKNOWN && !selection.shouldExit) {
        std::string input;
        std::cout << "\nSelect data structure (List, Heap, Graph, Custom)" << std::endl;
        std::cout << ">>> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            selection.shouldExit = true;
            break;
        }

        structure = parseStructure(input);
        if (structure == DataStructureEnum::UNKNOWN) {
            std::cout << "\nInvalid structure. Try again." << std::endl;
        }
    }

    selection.selectedStructure = structure;
    return selection;
}

InputManager::AlgorithmSelection InputManager::selectAlgorithm(DataStructureEnum structureType) {
    AlgorithmSelection selection;
    AlgorithmEnum algorithm = AlgorithmEnum::UNKNOWN;

    if (structureType == DataStructureEnum::UNKNOWN) {
        return selection;
    }

    while (algorithm == AlgorithmEnum::UNKNOWN && !selection.shouldExit) {
        std::string input;
        switch (structureType) {
            case DataStructureEnum::LIST:
                std::cout << "\nSelect algorithm (Insertion Sort, Merge Sort, Custom)" << std::endl;
                std::cout << ">>> ";
                break;
            case DataStructureEnum::HEAP:
                std::cout << "\nSelect algorithm (Heap Build, Heap Selection, Custom)" << std::endl;
                std::cout << ">>> ";
                break;
            case DataStructureEnum::GRAPH:
                std::cout << "\nSelect algorithm (A*, Prim, Custom)" << std::endl;
                std::cout << ">>> ";
                break;
            default:
                break;
        }

        std::getline(std::cin, input);

        if (input == "exit") {
            selection.shouldExit = true;
            break;
        }

        algorithm = parseAlgorithm(input);
        if (algorithm == AlgorithmEnum::UNKNOWN) {
            std::cout << "\nInvalid algorithm. Try again." << std::endl;
        }
    }

    selection.selectedAlgorithm = algorithm;
    return selection;
}

DataStructure* InputManager::createDataStructure(DataStructureEnum structureType) const {
    return DataStructureFactory::createDataStructure(structureType);
}

Algorithm* InputManager::createAlgorithm(AlgorithmEnum algorithmType) const {
    return AlgorithmFactory::createAlgorithm(algorithmType);
}

bool InputManager::populateDS(DataStructure* ds, DataStructureEnum structureType) {
    bool shouldExit = false;
    std::string input;

    if (structureType == DataStructureEnum::LIST || structureType == DataStructureEnum::HEAP) {
        std::cout << "\nInsert elements into structure ('done' to finish, 'rnd' to insert random elements, 'exit' to quit)" << std::endl;
        
        while (true) {
            std::cout << ">>> ";
            std::cin >> input;
            if (input == "done") break;
            if (input == "rnd") {
                std::cout << "\nHow many elements do you want to insert?" << std::endl;
                std::cout << ">>> ";
                std::cin >> input;

                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dist(0, 100000);

                for (int i = 0; i < std::stoi(input); ++i) {
                    int randomValue = dist(gen);
                    ds->insert(randomValue);
                }

                break;
            }
            if (input == "exit") {
                shouldExit = true;
                break;
            }

            try {
                int value = std::stoi(input);
                ds->insert(value);
            } catch (const std::invalid_argument&) {
                std::cout << "\nInvalid input. Please enter an integer or one of the commands." << std::endl;
            }
        }
    } else if (structureType == DataStructureEnum::GRAPH) {
        auto* graph = dynamic_cast<GraphStructure*>(ds);
        if (!graph) {
            std::cout << "\nFailed to access graph structure." << std::endl;
            return true;
        }

        graph->clear();

        std::cout << "\nInsert vertices into the graph ('done' to finish, 'exit' to quit)" << std::endl;
        while (true) {
            std::cout << ">>> ";
            std::cin >> input;

            if (input == "done") {
                break;
            }
            if (input == "exit") {
                shouldExit = true;
                break;
            }

            try {
                int vertex = std::stoi(input);
                graph->insert(vertex);
            } catch (const std::invalid_argument&) {
                std::cout << "\nInvalid vertex id. Please enter an integer or one of the commands." << std::endl;
            } catch (const std::out_of_range&) {
                std::cout << "\nVertex id out of range. Try a smaller value." << std::endl;
            }
        }

        if (shouldExit) {
            return shouldExit;
        }

        std::cout << "\nInsert edges as 'from to weight' (weight can be decimal)."
                     "\nType 'done' when finished, 'exit' to quit." << std::endl;
        while (true) {
            std::cout << ">>> ";
            std::cin >> input;

            if (input == "done") {
                break;
            }
            if (input == "exit") {
                shouldExit = true;
                break;
            }

            std::string toStr;
            std::string weightStr;
            if (!(std::cin >> toStr >> weightStr)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nInvalid edge format. Please enter three values: from to weight." << std::endl;
                continue;
            }

            try {
                int from = std::stoi(input);
                int to = std::stoi(toStr);
                double weight = std::stod(weightStr);
                graph->addEdge(from, to, weight, true);
            } catch (const std::invalid_argument&) {
                std::cout << "\nInvalid edge input. Please use integers for vertices and numeric weights." << std::endl;
            } catch (const std::out_of_range&) {
                std::cout << "\nEdge values out of range. Try smaller numbers." << std::endl;
            }
        }

        if (shouldExit) {
            return shouldExit;
        }

        std::cout << "\nWould you like to add heuristic values for A*? (y/n)" << std::endl;
        std::cout << ">>> ";
        std::cin >> input;

        if (input == "exit") {
            shouldExit = true;
        } else {
            std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) {
                return static_cast<char>(std::tolower(c));
            });

            if (input == "y" || input == "yes") {
                std::cout << "\nEnter heuristics as 'vertex value' ('done' to finish, 'exit' to quit)" << std::endl;
                while (true) {
                    std::cout << ">>> ";
                    std::cin >> input;

                    if (input == "done") {
                        break;
                    }
                    if (input == "exit") {
                        shouldExit = true;
                        break;
                    }

                    std::string valueStr;
                    if (!(std::cin >> valueStr)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "\nInvalid heuristic format. Please enter two values: vertex value." << std::endl;
                        continue;
                    }

                    try {
                        int vertex = std::stoi(input);
                        double heuristic = std::stod(valueStr);
                        graph->setHeuristic(vertex, heuristic);
                    } catch (const std::invalid_argument&) {
                        std::cout << "\nInvalid heuristic input. Please use an integer vertex and numeric value." << std::endl;
                    } catch (const std::out_of_range&) {
                        std::cout << "\nHeuristic input out of range. Try smaller numbers." << std::endl;
                    }
                }
            }
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return shouldExit;
}
