// include/model/DataStructure.hpp
#pragma once
#include <vector>

class DataStructure {
public:
    virtual ~DataStructure() = default;

    // Basic operations
    virtual void insert(int value) = 0;    // always at start
    virtual void remove(int value) = 0;

    // Retrieval operation used when displaying contents
    virtual std::vector<int> getElements() const = 0;
};
