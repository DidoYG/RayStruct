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
    // Runs the algorithm silently so benchmarks only capture timing.
    virtual void execute(DataStructure* ds) = 0;
    // Runs the algorithm while printing any relevant steps/results.
    virtual void executeAndDisplay(DataStructure* ds) = 0;
    // Shared helper for visual algorithms to print their state.
    virtual void display(const std::vector<int>& elements) = 0;

    // For returning the name of the algorithm when benchmarking.
    virtual std::string getName() const = 0;
};
