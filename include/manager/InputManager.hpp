// include/manager/InputManager.hpp
#pragma once
#include <string>
#include <algorithm>
#include "../structure/DataStructureFactory.hpp"
#include "../algorithm/AlgorithmFactory.hpp"

class InputManager {
private:
    std::string trim(const std::string& str);
    DataStructureEnum parseStructure(const std::string& input);
    AlgorithmEnum parseAlgorithm(const std::string& input);
    bool promptCustomAlgorithmPath(std::string& outPath, std::string& compilerOutput, std::string& libraryPath);
    bool validateCustomAlgorithmFile(const std::string& filePath, std::string& errorMessage);
    bool compileCustomAlgorithm(const std::string& filePath, std::string& compilerOutput, std::string& libraryPath);

public:
    struct StructureSelection {
        DataStructureEnum selectedStructure = DataStructureEnum::UNKNOWN;
        bool shouldExit = false;
    };

    struct AlgorithmSelection {
        AlgorithmEnum selectedAlgorithm = AlgorithmEnum::UNKNOWN;
        bool shouldExit = false;
        std::string customAlgorithmPath;
        std::string customAlgorithmCompileOutput;
        std::string customAlgorithmLibraryPath;
    };

    StructureSelection selectStructure();
    AlgorithmSelection selectAlgorithm(DataStructureEnum structureType);

    DataStructure* createDataStructure(DataStructureEnum structureType) const;
    Algorithm* createAlgorithm(const AlgorithmSelection& selection) const;

    bool populateDS(DataStructure* ds, DataStructureEnum structureType);
};
