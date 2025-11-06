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

// Namespace alias for filesystem
namespace fs = std::filesystem;

// Helper: trim whitespace from both ends of a string
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

// Helper: prompt user for custom data structure file path and handle compilation
bool InputManager::promptCustomStructurePath(std::string& outPath, std::string& compilerOutput, std::string& libraryPath) {
    while (true) {
        std::cout << "\nEnter the path to your custom data structure .cpp file ('exit' to quit)" << std::endl;
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

        // Check if file exists and is a regular file
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

        // Ensure it's a .cpp file
        std::string extension = candidate.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
        if (extension != ".cpp") {
            std::cout << "\nCustom data structure must be provided as a .cpp file. Try again." << std::endl;
            continue;
        }

        // Normalize path
        fs::path normalized = candidate;
        if (candidate.is_relative()) {
            auto absolutePath = fs::absolute(candidate, ec);
            if (!ec) normalized = absolutePath.lexically_normal();
        } else {
            normalized = candidate.lexically_normal();
        }

        // Validate file contents
        std::string validationError;
        if (!validateCustomStructureFile(normalized.string(), validationError)) {
            std::cout << "\nCustom data structure file failed validation: " << validationError << std::endl;
            std::cout << "Please adjust the file and try again." << std::endl;
            continue;
        }

        // Compile
        std::string compileOutput;
        std::string compiledLibraryPath;
        if (!compileCustomStructure(normalized.string(), compileOutput, compiledLibraryPath)) {
            std::cout << "\nCompilation of custom data structure failed." << std::endl;
            if (!compileOutput.empty()) std::cout << compileOutput << std::endl;
            std::cout << "Please address the compiler errors and try again." << std::endl;
            continue;
        }

        // Compilation succeeded
        if (!compileOutput.empty()) {
            std::cout << "\nCompilation succeeded with messages:\n" << compileOutput << std::endl;
        } else {
            std::cout << "\nCompilation succeeded with no warnings." << std::endl;
        }

        // Output results
        outPath = normalized.string();
        compilerOutput = compileOutput;
        libraryPath = compiledLibraryPath;
        std::cout << "\nCustom data structure path accepted: " << outPath << std::endl;
        std::cout << "Generated shared library: " << libraryPath << std::endl;
        return true;
    }
}

// Helper: validate the contents of the custom data structure file
bool InputManager::validateCustomStructureFile(const std::string& filePath, std::string& errorMessage) {
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

    // Convert to lowercase for uniform checking
    std::string lowercaseContent = content;
    std::transform(lowercaseContent.begin(), lowercaseContent.end(), lowercaseContent.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });

    // Check for required include
    if (lowercaseContent.find("datastructure.hpp") == std::string::npos) {
        errorMessage = "missing include for DataStructure.hpp.";
        return false;
    }

    // Ensure class inherits from DataStructure
    if (lowercaseContent.find("public datastructure") == std::string::npos) {
        errorMessage = "no class derives from the DataStructure base class (expecting 'public DataStructure').";
        return false;
    }

    // Required method overrides
    bool hasInsert = lowercaseContent.find("void insert(") != std::string::npos;
    bool hasRemove = lowercaseContent.find("void remove(") != std::string::npos;
    bool hasGetElements = lowercaseContent.find("getelements(") != std::string::npos;

    if (!hasInsert || !hasRemove || !hasGetElements) {
        errorMessage = "missing required method overrides (insert, remove, getElements).";
        return false;
    }

    // Check for factory method
    if (lowercaseContent.find("createdatastructure(") == std::string::npos) {
        errorMessage = "missing factory function 'createDataStructure'.";
        return false;
    }

    return true;
}

// Helper: compile the custom data structure .cpp file into a shared library
bool InputManager::compileCustomStructure(const std::string& filePath, std::string& compilerOutput, std::string& libraryPath) {
    fs::path sourcePath(__FILE__);
    fs::path projectRoot = sourcePath.parent_path().parent_path().parent_path();
    fs::path includeDir = projectRoot / "include";

    // Ensure output directory exists
    fs::path dsDir = projectRoot / "src" / "structure";
    std::error_code dirEc;
    fs::create_directories(dsDir, dirEc);

    // Prepare output path
    fs::path inputPath(filePath);
    std::string stem = inputPath.stem().string();
    if (stem.empty()) stem = "custom_datastructure";

    // Sanitize filename
    std::replace_if(stem.begin(), stem.end(), [](unsigned char c) {
        return !std::isalnum(static_cast<unsigned char>(c)) && c != '_';
    }, '_');

    fs::path libraryFile = dsDir / (stem + "_custom.so");
    std::error_code removeEc;
    fs::remove(libraryFile, removeEc);

    fs::path logPath = "raystruct_custom_ds.txt";

    // Build g++ command
    std::ostringstream cmdStream;
    cmdStream << "g++ -std=c++17 -shared -fPIC -Wall -Wextra"
              << " -I\"" << includeDir.string() << "\""
              << " -I\"" << (projectRoot / "src").string() << "\""
              << " \"" << filePath << "\""
              << " -o \"" << libraryFile.string() << "\""
              << " >\"" << logPath.string() << "\" 2>&1";

    int result = std::system(cmdStream.str().c_str());

    // Read compiler output
    compilerOutput.clear();
    {
        std::ifstream logStream(logPath);
        if (logStream.is_open()) {
            std::ostringstream oss;
            oss << logStream.rdbuf();
            compilerOutput = oss.str();
        }
    }

    fs::remove(logPath, removeEc);

    if (result != 0) {
        fs::remove(libraryFile, removeEc);
        return false;
    }

    libraryPath = libraryFile.string();
    return true;
}

// Helper: prompt user for custom algorithm file path and handle compilation
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

        // Check if file exists and is a regular file
        fs::path candidate(rawPath);
        std::error_code ec;
        if (!fs::exists(candidate, ec) || ec) {
            std::cout << "\nFile not found: " << rawPath << "\nPlease provide a valid path." << std::endl;
            continue;
        }

        // Check if it's a regular file
        if (!fs::is_regular_file(candidate, ec) || ec) {
            std::cout << "\nProvided path is not a regular file. Try again." << std::endl;
            continue;
        }

        // Check file extension
        std::string extension = candidate.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });

        if (extension != ".cpp") {
            std::cout << "\nCustom algorithm must be provided as a .cpp file. Try again." << std::endl;
            continue;
        }

        // Normalize path
        fs::path normalized = candidate;
        if (candidate.is_relative()) {
            auto absolutePath = fs::absolute(candidate, ec);
            if (!ec) {
                normalized = absolutePath.lexically_normal();
            }
        } else {
            normalized = candidate.lexically_normal();
        }

        // Validate file contents
        std::string validationError;
        if (!validateCustomAlgorithmFile(normalized.string(), validationError)) {
            std::cout << "\nCustom algorithm file failed validation: " << validationError << std::endl;
            std::cout << "Please adjust the file and try again." << std::endl;
            continue;
        }

        // Compile the custom algorithm
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

        // Compilation succeeded
        if (!compileOutput.empty()) {
            std::cout << "\nCompilation succeeded with messages:\n" << compileOutput << std::endl;
        } else {
            std::cout << "\nCompilation succeeded with no warnings." << std::endl;
        }

        // Set output parameters
        outPath = normalized.string();
        compilerOutput = compileOutput;
        libraryPath = compiledLibraryPath;
        std::cout << "\nCustom algorithm path accepted: " << outPath << std::endl;
        std::cout << "Generated shared library: " << libraryPath << std::endl;
        return true;
    }
}

// Helper: validate the contents of the custom algorithm file
bool InputManager::validateCustomAlgorithmFile(const std::string& filePath, std::string& errorMessage) {
    // Basic validation: check for required includes and method signatures
    std::ifstream file(filePath);
    if (!file.is_open()) {
        errorMessage = "unable to open file.";
        return false;
    }

    // Read entire file content
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    if (content.empty()) {
        errorMessage = "file appears to be empty.";
        return false;
    }

    // Convert content to lowercase for case-insensitive checks
    std::string lowercaseContent = content;
    std::transform(lowercaseContent.begin(), lowercaseContent.end(), lowercaseContent.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });

    // Check for required elements
    // Check for include of Algorithm.hpp
    if (lowercaseContent.find("algorithm.hpp") == std::string::npos) {
        errorMessage = "missing include for Algorithm.hpp.";
        return false;
    }

    // Check for class deriving from Algorithm
    bool inheritsFromAlgorithm = lowercaseContent.find("public algorithm") != std::string::npos;
    if (!inheritsFromAlgorithm) {
        errorMessage = "no class derives from the Algorithm base class (expecting 'public Algorithm').";
        return false;
    }

    // Check for required method overrides
    bool hasExecute = lowercaseContent.find("void execute(") != std::string::npos;
    bool hasExecuteAndDisplay = lowercaseContent.find("void executeanddisplay(") != std::string::npos;
    bool hasDisplay = lowercaseContent.find("void display(") != std::string::npos;

    // Ensure all required methods are present
    if (!hasExecute || !hasExecuteAndDisplay || !hasDisplay) {
        errorMessage = "missing required method overrides (execute, executeAndDisplay, display).";
        return false;
    }

    // Check for factory function that will be used for linkage at runtime
    if (lowercaseContent.find("createalgorithm(") == std::string::npos) {
        errorMessage = "missing factory function 'createAlgorithm'.";
        return false;
    }

    return true;
}

// Helper: compile the custom algorithm .cpp file into a shared library
bool InputManager::compileCustomAlgorithm(const std::string& filePath, std::string& compilerOutput, std::string& libraryPath) {
    // Determine project root and include paths
    fs::path sourcePath(__FILE__);
    fs::path projectRoot = sourcePath.parent_path().parent_path().parent_path();
    fs::path includeDir = projectRoot / "include";

    // Ensure output directory exists
    fs::path algorithmDir = projectRoot / "src" / "algorithm";
    std::error_code dirEc;
    fs::create_directories(algorithmDir, dirEc);

    // Prepare output library path
    fs::path inputPath(filePath);
    std::string stem = inputPath.stem().string();
    if (stem.empty()) {
        stem = "custom_algorithm";
    }

    // Sanitize stem to be filesystem-friendly
    std::replace_if(stem.begin(), stem.end(), [](unsigned char c) {
        return !std::isalnum(static_cast<unsigned char>(c)) && c != '_';
    }, '_');

    // Remove any existing library file
    fs::path libraryFile = algorithmDir / (stem + "_custom.so");
    std::error_code removeEc;
    fs::remove(libraryFile, removeEc);

    // Log file for compiler output
    fs::path logPath = "raystruct_custom_algo.txt";

    // Construct compilation command
    std::ostringstream cmdStream;
    cmdStream << "g++ -std=c++17 -shared -fPIC -Wall -Wextra"
              << " -I\"" << includeDir.string() << "\""
              << " -I\"" << (projectRoot / "src").string() << "\""
              << " \"" << filePath << "\""
              << " -o \"" << libraryFile.string() << "\""
              << " >\"" << logPath.string() << "\" 2>&1";

    // Execute compilation command
    int result = std::system(cmdStream.str().c_str());

    // Read compiler output from log file
    compilerOutput.clear();
    {
        std::ifstream logStream(logPath);
        if (logStream.is_open()) {
            std::ostringstream oss;
            oss << logStream.rdbuf();
            compilerOutput = oss.str();
        }
    }

    // Clean up log file
    std::error_code ec;
    fs::remove(logPath, ec);

    // Check compilation result
    if (result != 0) {
        fs::remove(libraryFile, ec);
        return false;
    }

    // Compilation succeeded
    libraryPath = libraryFile.string();
    return true;
}

// Public method: prompt user to select data structure
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

        // Parse structure and validate
        structure = parseStructure(input);
        if (structure == DataStructureEnum::UNKNOWN) {
            std::cout << "\nInvalid structure. Try again." << std::endl;
        }
        if (structure == DataStructureEnum::CUSTOM) {
            std::cout << "\nSelected custom algorithm." << std::endl;
            std::cout << "Ensure your implementation derives from the Algorithm base class provided by RayStruct++." << std::endl;
            if (!promptCustomStructurePath(selection.customStructurePath, selection.customStructureCompileOutput, selection.customStructureLibraryPath)) {
               selection.shouldExit = true;
               structure = DataStructureEnum::UNKNOWN;  
            }
        }
    }

    selection.selectedStructure = structure;
    return selection;
}

// Public method: prompt user to select algorithm based on chosen data structure
InputManager::AlgorithmSelection InputManager::selectAlgorithm(DataStructureEnum structureType) {
    AlgorithmSelection selection;
    AlgorithmEnum algorithm = AlgorithmEnum::UNKNOWN;
    bool shouldSkipSelection = false;

    if (structureType == DataStructureEnum::UNKNOWN) {
        return selection;
    }

    // Prompt for algorithm selection based on structure type
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
            case DataStructureEnum::CUSTOM:
                shouldSkipSelection = true;
                algorithm = AlgorithmEnum::CUSTOM;
                break;
            default:
                break;
        }

        // Skip algorithm selection if the selected structure is Custom
        if (!shouldSkipSelection) {
            std::getline(std::cin, input);

            if (input == "exit") {
                selection.shouldExit = true;
                break;
            }

            // Parse algorithm and validate
            algorithm = parseAlgorithm(input);
            if (algorithm == AlgorithmEnum::UNKNOWN) {
                std::cout << "\nInvalid algorithm. Try again." << std::endl;
            }
        }
        // Handle custom algorithm selection
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

// Public method: create data structure instance using factory
DataStructure* InputManager::createDataStructure(const StructureSelection& selection) const {
    return DataStructureFactory::createDataStructure(selection.selectedStructure, selection.customStructureLibraryPath);
}

// Public method: create algorithm instance using factory
Algorithm* InputManager::createAlgorithm(const AlgorithmSelection& selection) const {
    return AlgorithmFactory::createAlgorithm(selection.selectedAlgorithm, selection.customAlgorithmLibraryPath);
}

// Public method: populate data structure with user-provided data
bool InputManager::populateDS(DataStructure* ds, DataStructureEnum structureType) {
    bool shouldExit = false;
    std::string input;

    if (structureType == DataStructureEnum::LIST || structureType == DataStructureEnum::HEAP || structureType == DataStructureEnum::CUSTOM) {
        std::cout << "\nInsert elements into structure ('done' to finish, 'rnd' to insert random elements, 'exit' to quit)" << std::endl;
        
        // Input loop for inserting elements
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

            // Attempt to convert input to integer and insert
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

        // Input loop for inserting vertices
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

            // Attempt to convert input to integer and insert vertex
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

        // Input loop for inserting edges
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

            // Attempt to convert and add edge
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

        // Input loop for adding heuristics
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

                    // Attempt to convert and set heuristic
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

    // Clear any remaining input
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return shouldExit;
}
