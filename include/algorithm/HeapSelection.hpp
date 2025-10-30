// include/algorithm/HeapSelection.hpp
#pragma once
#include "../model/Algorithm.hpp"

class HeapSelection : public Algorithm {
private:
    // Heap selection parameters
    bool isSmallest;
    int k;
    int result;
    
    // Helper functions
    void display(const std::vector<int>& elements) override;
    int findKthSmallest(std::vector<int>& elements, int k);
    int findKthLargest(std::vector<int>& elements, int k);

public:
    // Overrides from Algorithm
    void execute(DataStructure* ds) override;
    void executeAndDisplay(DataStructure* ds) override;

    // Setters for parameters
    void setIsSmallest(bool choice);
    void setK(int k);
};
