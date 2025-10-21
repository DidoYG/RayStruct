// include/algorithm/AlgorithmFactory.hpp
#pragma once
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
    static Algorithm* createAlgorithm(AlgorithmEnum type);
};
