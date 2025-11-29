// include/model/DataStructure.hpp
#pragma once
#include <vector>
#include <string>

class DataStructure {
public:
    virtual ~DataStructure() = default;

    // Basic operations
    // Adds a value to the structure using its custom insertion semantics.
    virtual void insert(int value) = 0;
    // Removes a value (or values) using structure-specific logic.
    virtual void remove(int value) = 0;

    // Retrieval operation used when displaying contents
    // Provides a snapshot of the structure's elements for algorithms/UI.
    virtual std::vector<int> getElements() const = 0;

    // For returning the name of the data structure when benchmarking.
    virtual std::string getName() const = 0;
};
