// include/model/Algorithm.hpp
#pragma once
#include <string>
#include "DataStructure.hpp"

// Base interface for all algorithms
class Algorithm {
public:
    virtual ~Algorithm() = default;
    virtual void execute(DataStructure* ds) = 0;
    virtual void executeAndDisplay(DataStructure* ds) = 0;
    virtual void display(const std::vector<int>& elements) = 0;
};

