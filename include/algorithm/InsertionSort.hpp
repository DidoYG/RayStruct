// include/algorithm/InsertionSort.hpp
#pragma once
#include "../model/Algorithm.hpp"

class InsertionSort : public Algorithm {
private:
    // Overrides from Algorithm
    void display(const std::vector<int>& elements) override;
    
public:
    // Overrides from Algorithm
    void execute(DataStructure* ds) override;
    void executeAndDisplay(DataStructure* ds) override;
    std::string getName() const override;
};