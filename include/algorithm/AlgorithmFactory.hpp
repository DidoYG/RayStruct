// include/algorithm/AlgorithmFactory.hpp
#pragma once
#include <string>
#include "../core/Enums.hpp"
#include "../model/Algorithm.hpp"
#include "InsertionSort.hpp"
#include "MergeSort.hpp"
#include "HeapBuild.hpp"
#include "HeapSelection.hpp"
#include "AStar.hpp"
#include "Prims.hpp"

class AlgorithmFactory {
public:
    // Instantiates built-in algorithms or loads a custom shared library when requested.
    static Algorithm* createAlgorithm(AlgorithmEnum type, const std::string& customLibraryPath = "");
    // Clears all previously loaded custom algorithm libraries.
    static void cleanupCustomLibraries();
};
