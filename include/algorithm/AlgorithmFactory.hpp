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
    // Factory method to create algorithm instances based on the provided enum type
    // For CUSTOM type, a customLibraryPath must be provided.
    static Algorithm* createAlgorithm(AlgorithmEnum type, const std::string& customLibraryPath = "");
    static void cleanupCustomLibraries();
};
