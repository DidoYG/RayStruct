// include/algorithm/InsertionSort.hpp
#pragma once
#include "../model/Algorithm.hpp"

class InsertionSort : public Algorithm {
private:
    // Overrides from Algorithm
    // Prints the array contents for visualization.
    void display(const std::vector<int>& elements) override;
    
public:
    // Overrides from Algorithm
    // Executes insertion sort without printing steps.
    void execute(DataStructure* ds) override;
    // Executes insertion sort while showing progress.
    void executeAndDisplay(DataStructure* ds) override;
    // Returns the algorithm name for menus/output.
    std::string getName() const override;
};
