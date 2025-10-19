// src/Algorithm/AlgorithmFactory.cpp
#include "../../include/algorithm/AlgorithmFactory.hpp"

Algorithm* AlgorithmFactory::createAlgorithm(AlgorithmEnum type) {
    switch (type) {
        case AlgorithmEnum::INSERTION_SORT: 
            return new InsertionSort();
            break;
        case AlgorithmEnum::MERGE_SORT: 
            return new MergeSort();
            break;
        case AlgorithmEnum::HEAP_BUILD: 
            return new HeapBuild();
            break;
        case AlgorithmEnum::HEAP_SELECTION:
            //return new MergeSort();
            break;
        case AlgorithmEnum::A_STAR: 
            // return new GraphAStar();
            break;
        case AlgorithmEnum::PRIMS: 
            // return new GraphBStar();
            break;
        case AlgorithmEnum::CUSTOM:
            // return custom
            break;
        default:
            return nullptr;
    }
    return nullptr;
}