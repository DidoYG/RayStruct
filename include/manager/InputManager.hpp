// include/manager/InputManager.hpp
#pragma once
#include <string>
#include <algorithm>
#include "../structure/DataStructureFactory.hpp"
#include "../algorithm/AlgorithmFactory.hpp"

class InputManager {
private:
    // Helper functions
    std::string trim(const std::string& str);
    DataStructureEnum parseStructure(const std::string& input);
    AlgorithmEnum parseAlgorithm(const std::string& input);
    bool promptCustomStructurePath(std::string& outPath, std::string& compilerOutput, std::string& libraryPath);
    bool validateCustomStructureFile(const std::string& filePath, std::string& errorMessage);
    bool compileCustomStructure(const std::string& filePath, std::string& compilerOutput, std::string& libraryPath);
    bool promptCustomAlgorithmPath(std::string& outPath, std::string& compilerOutput, std::string& libraryPath);
    bool validateCustomAlgorithmFile(const std::string& filePath, std::string& errorMessage);
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
    StructureSelection selectStructure();
    AlgorithmSelection selectAlgorithm(DataStructureEnum structureType);

    // Public methods for calling the factories
    DataStructure* createDataStructure(const StructureSelection& selection) const;
    Algorithm* createAlgorithm(const AlgorithmSelection& selection) const;

    // Method to populate data structure with initial data
    bool populateDS(DataStructure* ds, DataStructureEnum structureType);
};
