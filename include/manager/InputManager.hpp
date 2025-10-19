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
    DataStructureEnum structure;
    AlgorithmEnum algorithm;

    std::string trim(const std::string& str);
    DataStructureEnum parseStructure(const std::string& input);
    AlgorithmEnum parseAlgorithm(const std::string& input);

    struct InputResult {
        DataStructureEnum selectedStructure;
        AlgorithmEnum selectedAlgorithm;
        bool shouldExit;
    };

    InputResult result;

public:
    InputResult selectStructureAndAlgorithm();

    bool populateDS(DataStructure* ds); // Placeholder for future implementation
};
