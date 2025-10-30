// include/model/Algorithm.hpp
#pragma once
#include <string>
#include "DataStructure.hpp"

// Base interface for all algorithms
class Algorithm {
public:
    // Virtual destructor for proper cleanup of derived classes
    virtual ~Algorithm() = default;

    // Base methods to be implemented by derived algorithm classes
    virtual void execute(DataStructure* ds) = 0;
    virtual void executeAndDisplay(DataStructure* ds) = 0;
    virtual void display(const std::vector<int>& elements) = 0;
};

