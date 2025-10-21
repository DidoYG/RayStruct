// src/Algorithm/AlgorithmFactory.cpp
#include "../../include/algorithm/AlgorithmFactory.hpp"

Algorithm* AlgorithmFactory::createAlgorithm(AlgorithmEnum type) {
    switch (type) {
        case AlgorithmEnum::INSERTION_SORT: 
            return new InsertionSort();
        case AlgorithmEnum::MERGE_SORT: 
            return new MergeSort();
        case AlgorithmEnum::HEAP_BUILD: 
            return new HeapBuild();
        case AlgorithmEnum::HEAP_SELECTION:
            return new HeapSelection();
        case AlgorithmEnum::A_STAR: 
            return new AStar();
        case AlgorithmEnum::PRIMS: 
            return new Prims();
        case AlgorithmEnum::CUSTOM:
            // return custom
            break;
        default:
            return nullptr;
    }
    return nullptr;
}
