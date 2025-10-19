// src/DataStructureFactory.cpp
#include "../../include/structure/DataStructureFactory.hpp"

DataStructure* DataStructureFactory::createDataStructure(DataStructureEnum type) {
    switch (type) {
        case DataStructureEnum::LIST: return new ListStructure();
        case DataStructureEnum::HEAP: return new HeapStructure();
        case DataStructureEnum::GRAPH: return new GraphStructure();
        default: return nullptr;
    }
}