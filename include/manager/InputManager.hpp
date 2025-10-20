// include/manager/InputManager.hpp
#pragma once
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <random>
#include <limits>
#include "../structure/DataStructureFactory.hpp"
#include "../algorithm/AlgorithmFactory.hpp"

class InputManager {
private:
    std::string trim(const std::string& str);
    DataStructureEnum parseStructure(const std::string& input);
    AlgorithmEnum parseAlgorithm(const std::string& input);

public:
    struct StructureSelection {
        DataStructureEnum selectedStructure = DataStructureEnum::UNKNOWN;
        bool shouldExit = false;
    };

    struct AlgorithmSelection {
        AlgorithmEnum selectedAlgorithm = AlgorithmEnum::UNKNOWN;
        bool shouldExit = false;
    };

    StructureSelection selectStructure();
    AlgorithmSelection selectAlgorithm(DataStructureEnum structureType);

    DataStructure* createDataStructure(DataStructureEnum structureType) const;
    Algorithm* createAlgorithm(AlgorithmEnum algorithmType) const;

    bool populateDS(DataStructure* ds, DataStructureEnum structureType);
};
