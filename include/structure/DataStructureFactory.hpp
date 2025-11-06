// include/structure/DataStructureFactory.h
#pragma once
#include <string>
#include "../model/DataStructure.hpp"
#include "../structure/ListStructure.hpp"
#include "../structure/HeapStructure.hpp"
#include "../structure/GraphStructure.hpp"
#include "../core/Enums.hpp"

class DataStructureFactory {
public:
    // Factory method to create data structure instances based on the provided enum type
    // For CUSTOM type, a customLibraryPath must be provided.
    static DataStructure* createDataStructure(DataStructureEnum type, const std::string& customLibraryPath = "");
    static void cleanupCustomLibraries();
};
