// include/model/DataStructure.hpp
#pragma once
#include <vector>

class DataStructure {
public:
    virtual ~DataStructure() = default;

    // Basic modification
    virtual void insert(int value) = 0;    // always at start
    virtual void remove(int value) = 0;    // some may ignore if not supported

    // Visualization support
    virtual std::vector<int> getElements() const = 0;
};
