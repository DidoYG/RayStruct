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
    // Creates built-in data structures or custom ones when CUSTOM is requested.
    static DataStructure* createDataStructure(DataStructureEnum type, const std::string& customLibraryPath = "");
    // Clears all dynamically loaded custom data structure libraries.
    static void cleanupCustomLibraries();
};
