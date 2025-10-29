// src/manager/InputManager.cpp
#include "../../include/manager/InputManager.hpp"
#include "../../include/structure/GraphStructure.hpp"
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

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

bool InputManager::promptCustomAlgorithmPath(std::string& outPath, std::string& compilerOutput, std::string& libraryPath) {
    while (true) {
        std::cout << "\nEnter the path to your custom algorithm .cpp file ('exit' to quit)" << std::endl;
        std::cout << ">>> ";

        std::string rawPath;
        std::getline(std::cin, rawPath);
        rawPath = trim(rawPath);

        if (rawPath.empty()) {
            std::cout << "\nPath cannot be empty. Try again." << std::endl;
            continue;
        }

        if (rawPath == "exit") {
            return false;
        }

        fs::path candidate(rawPath);
        std::error_code ec;
        if (!fs::exists(candidate, ec) || ec) {
            std::cout << "\nFile not found: " << rawPath << "\nPlease provide a valid path." << std::endl;
            continue;
        }

        if (!fs::is_regular_file(candidate, ec) || ec) {
            std::cout << "\nProvided path is not a regular file. Try again." << std::endl;
            continue;
        }

        std::string extension = candidate.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });

        if (extension != ".cpp") {
            std::cout << "\nCustom algorithm must be provided as a .cpp file. Try again." << std::endl;
            continue;
        }

        fs::path normalized = candidate;
        if (candidate.is_relative()) {
            auto absolutePath = fs::absolute(candidate, ec);
            if (!ec) {
                normalized = absolutePath.lexically_normal();
            }
        } else {
            normalized = candidate.lexically_normal();
        }

        std::string validationError;
        if (!validateCustomAlgorithmFile(normalized.string(), validationError)) {
            std::cout << "\nCustom algorithm file failed validation: " << validationError << std::endl;
            std::cout << "Please adjust the file and try again." << std::endl;
            continue;
        }

        std::string compileOutput;
        std::string compiledLibraryPath;
        if (!compileCustomAlgorithm(normalized.string(), compileOutput, compiledLibraryPath)) {
            std::cout << "\nCompilation of custom algorithm failed." << std::endl;
            if (!compileOutput.empty()) {
                std::cout << compileOutput << std::endl;
            }
            std::cout << "Please address the compiler errors and try again." << std::endl;
            continue;
        }

        if (!compileOutput.empty()) {
            std::cout << "\nCompilation succeeded with messages:\n" << compileOutput << std::endl;
        } else {
            std::cout << "\nCompilation succeeded with no warnings." << std::endl;
        }

        outPath = normalized.string();
        compilerOutput = compileOutput;
        libraryPath = compiledLibraryPath;
        std::cout << "\nCustom algorithm path accepted: " << outPath << std::endl;
        std::cout << "Generated shared library: " << libraryPath << std::endl;
        return true;
    }
}

bool InputManager::validateCustomAlgorithmFile(const std::string& filePath, std::string& errorMessage) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        errorMessage = "unable to open file.";
        return false;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    if (content.empty()) {
        errorMessage = "file appears to be empty.";
        return false;
    }

    std::string lowercaseContent = content;
    std::transform(lowercaseContent.begin(), lowercaseContent.end(), lowercaseContent.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });

    if (lowercaseContent.find("algorithm.hpp") == std::string::npos) {
        errorMessage = "missing include for Algorithm.hpp.";
        return false;
    }

    bool inheritsFromAlgorithm = lowercaseContent.find("public algorithm") != std::string::npos;
    if (!inheritsFromAlgorithm) {
        errorMessage = "no class derives from the Algorithm base class (expecting 'public Algorithm').";
        return false;
    }

    bool hasExecute = lowercaseContent.find("void execute(") != std::string::npos;
    bool hasExecuteAndDisplay = lowercaseContent.find("void executeanddisplay(") != std::string::npos;
    bool hasDisplay = lowercaseContent.find("void display(") != std::string::npos;

    if (!hasExecute || !hasExecuteAndDisplay || !hasDisplay) {
        errorMessage = "missing required method overrides (execute, executeAndDisplay, display).";
        return false;
    }

    if (lowercaseContent.find("createalgorithm(") == std::string::npos) {
        errorMessage = "missing factory function 'createAlgorithm'.";
        return false;
    }

    return true;
}

bool InputManager::compileCustomAlgorithm(const std::string& filePath, std::string& compilerOutput, std::string& libraryPath) {
    fs::path sourcePath(__FILE__);
    fs::path projectRoot = sourcePath.parent_path().parent_path().parent_path();
    fs::path includeDir = projectRoot / "include";

    fs::path algorithmDir = projectRoot / "src" / "algorithm";
    std::error_code dirEc;
    fs::create_directories(algorithmDir, dirEc);

    fs::path inputPath(filePath);
    std::string stem = inputPath.stem().string();
    if (stem.empty()) {
        stem = "custom_algorithm";
    }
    std::replace_if(stem.begin(), stem.end(), [](unsigned char c) {
        return !std::isalnum(static_cast<unsigned char>(c)) && c != '_';
    }, '_');

    fs::path libraryFile = algorithmDir / ("lib" + stem + "_custom.so");
    std::error_code removeEc;
    fs::remove(libraryFile, removeEc);

    fs::path logPath = "raystruct_custom_algo.txt";

    std::ostringstream cmdStream;
    cmdStream << "g++ -std=c++20 -shared -fPIC -Wall -Wextra"
              << " -I\"" << includeDir.string() << "\""
              << " -I\"" << (projectRoot / "src").string() << "\""
              << " \"" << filePath << "\""
              << " -o \"" << libraryFile.string() << "\""
              << " >\"" << logPath.string() << "\" 2>&1";

    int result = std::system(cmdStream.str().c_str());

    compilerOutput.clear();
    {
        std::ifstream logStream(logPath);
        if (logStream.is_open()) {
            std::ostringstream oss;
            oss << logStream.rdbuf();
            compilerOutput = oss.str();
        }
    }

    std::error_code ec;
    fs::remove(logPath, ec);

    if (result != 0) {
        fs::remove(libraryFile, ec);
        return false;
    }

    libraryPath = libraryFile.string();
    return true;
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
        if (algorithm == AlgorithmEnum::CUSTOM) {
            std::cout << "\nSelected custom algorithm." << std::endl;
            std::cout << "Ensure your implementation derives from the Algorithm base class provided by RayStruct++." << std::endl;
            if (!promptCustomAlgorithmPath(selection.customAlgorithmPath, selection.customAlgorithmCompileOutput, selection.customAlgorithmLibraryPath)) {
                selection.shouldExit = true;
                algorithm = AlgorithmEnum::UNKNOWN;
            }
        }
    }

    selection.selectedAlgorithm = algorithm;
    return selection;
}

DataStructure* InputManager::createDataStructure(DataStructureEnum structureType) const {
    return DataStructureFactory::createDataStructure(structureType);
}

Algorithm* InputManager::createAlgorithm(const AlgorithmSelection& selection) const {
    return AlgorithmFactory::createAlgorithm(selection.selectedAlgorithm, selection.customAlgorithmLibraryPath);
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
