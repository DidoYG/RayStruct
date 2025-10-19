// include/structure/DataStructureFactory.h
#pragma once
#include "../model/DataStructure.hpp"
#include "../structure/ListStructure.hpp"
#include "../structure/HeapStructure.hpp"
#include "../structure/GraphStructure.hpp"
#include "../core/Enums.hpp"

class DataStructureFactory {
public:
    static DataStructure* createDataStructure(DataStructureEnum type);
};
