// src/manager/InputManager.cpp
#include "../../include/manager/InputManager.hpp"

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
    if (s == "a*") return AlgorithmEnum::A_STAR;
    if (s == "prim") return AlgorithmEnum::PRIMS;
    if (s == "custom") return AlgorithmEnum::CUSTOM;

    return AlgorithmEnum::UNKNOWN;
}

InputManager::InputResult InputManager::selectStructureAndAlgorithm() {
    bool shouldExit = false;
    structure = DataStructureEnum::UNKNOWN;
    algorithm = AlgorithmEnum::UNKNOWN;

    while (!shouldExit) {
        std::string input;

        // Keep asking until a valid structure is selected
        while (structure == DataStructureEnum::UNKNOWN) {
            std::cout << "\nSelect data structure (List, Heap, Graph, Custom)" << std::endl;
            std::cout << ">>> ";
            std::getline(std::cin, input);
            if (input == "exit") { shouldExit = true; break; }

            structure = parseStructure(input);
            if (structure == DataStructureEnum::UNKNOWN) {
                std::cout << "\nInvalid structure. Try again." << std::endl;
            }
        }
        if (shouldExit) break;

        if (structure == DataStructureEnum::CUSTOM) {
            // add the logic for uploading a file 
        }

        // Keep asking until a valid algorithm is selected
        while (algorithm == AlgorithmEnum::UNKNOWN) {
            switch (structure)
            {
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
            if (input == "exit") { shouldExit = true; break; }

            algorithm = parseAlgorithm(input);
            if (algorithm == AlgorithmEnum::UNKNOWN) {
                std::cout << "\nInvalid algorithm. Try again." << std::endl;
            }
        }
        if (shouldExit) break;

        break;
    }

    result.selectedStructure = structure;
    result.selectedAlgorithm = algorithm;
    result.shouldExit = shouldExit;

    return result;
}

bool InputManager::populateDS(DataStructure* ds) {
    bool shouldExit = false;
    std::string input;

    if (result.selectedStructure == DataStructureEnum::LIST || result.selectedStructure == DataStructureEnum::HEAP) {
        while (true) {
            std::cout << "\nInsert elements into list ('done' to finish, 'rnd' to insert random elements, 'exit' to quit)" << std::endl;
            std::cout << ">>> ";
            std::cin >> input;
            if (input == "done") break;
            if (input == "rnd") {
                std::cout << "\nHow many elements do you wan to insert?" << std::endl;
                std::cout << ">>> ";
                std::cin >> input;

                std::random_device rd;
                std::mt19937 gen(rd()); // make a rnd method that checks time, converts it to binary them multiplys it to the input and so on
                std::uniform_int_distribution<> dist(0, 100);

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
    } else if (result.selectedStructure == DataStructureEnum::GRAPH) {
        // add the logic for populating a graph
    }

    return shouldExit;
}