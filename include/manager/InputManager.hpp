// include/manager/InputManager.hpp
#pragma once
#include <string>
#include <algorithm>
#include "../structure/DataStructureFactory.hpp"
#include "../algorithm/AlgorithmFactory.hpp"

class InputManager {
private:
    // Helper functions
    // Removes surrounding whitespace so menu parsing is consistent.
    std::string trim(const std::string& str);
    // Maps user text to a DataStructureEnum value.
    DataStructureEnum parseStructure(const std::string& input);
    // Maps user text to an AlgorithmEnum value.
    AlgorithmEnum parseAlgorithm(const std::string& input);
    // Ensures the requested algorithm can run on the chosen structure.
    bool isAlgorithmCompatible(AlgorithmEnum algorithm, DataStructureEnum structureType);
    // Handles validating and compiling a custom structure uploaded by the user.
    bool promptCustomStructurePath(std::string& outPath, std::string& compilerOutput, std::string& libraryPath);
    // Checks whether the custom structure file satisfies the base contract.
    bool validateCustomStructureFile(const std::string& filePath, std::string& errorMessage);
    // Compiles the validated custom structure into a shared library.
    bool compileCustomStructure(const std::string& filePath, std::string& compilerOutput, std::string& libraryPath);
    // Handles validating and compiling a custom algorithm uploaded by the user.
    bool promptCustomAlgorithmPath(std::string& outPath, std::string& compilerOutput, std::string& libraryPath);
    // Verifies that a custom algorithm derives from Algorithm and implements overrides.
    bool validateCustomAlgorithmFile(const std::string& filePath, std::string& errorMessage);
    // Compiles the validated custom algorithm into a shared library.
    bool compileCustomAlgorithm(const std::string& filePath, std::string& compilerOutput, std::string& libraryPath);

public:
    // Struct that saves the result of structure selection
    struct StructureSelection {
        DataStructureEnum selectedStructure = DataStructureEnum::UNKNOWN;
        bool shouldExit = false;
        std::string customStructurePath; // for custom structure
        std::string customStructureCompileOutput; // compiler output
        std::string customStructureLibraryPath; // path to compiled library
    };

    // Struct that saves the result of algorithm selection
    struct AlgorithmSelection {
        AlgorithmEnum selectedAlgorithm = AlgorithmEnum::UNKNOWN;
        bool shouldExit = false;
        std::string customAlgorithmPath; // for custom algorithms
        std::string customAlgorithmCompileOutput; // compiler output
        std::string customAlgorithmLibraryPath; // path to compiled library
    };

    // Public methods for user selection
    // Loops until a valid structure is provided or the user exits.
    StructureSelection selectStructure();
    // Presents compatible algorithms for the chosen structure.
    AlgorithmSelection selectAlgorithm(DataStructureEnum structureType);

    // Public methods for calling the factories
    // Instantiates either built-in or custom data structures.
    DataStructure* createDataStructure(const StructureSelection& selection) const;
    // Instantiates either built-in or custom algorithms.
    Algorithm* createAlgorithm(const AlgorithmSelection& selection) const;

    // Method to populate data structure with initial data
    // Launches the input loop for adding elements or graph edges.
    bool populateDS(DataStructure* ds, DataStructureEnum structureType);
};
