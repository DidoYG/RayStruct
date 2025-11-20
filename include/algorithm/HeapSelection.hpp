// include/algorithm/HeapSelection.hpp
#pragma once
#include "../model/Algorithm.hpp"

class HeapSelection : public Algorithm {
private:
    // Heap selection parameters
    bool isSmallest = true;
    int k = 1;
    int result = 0;
    
    // Helper functions
    void display(const std::vector<int>& elements) override;
    int findKthSmallest(std::vector<int>& elements, int k);
    int findKthLargest(std::vector<int>& elements, int k);

public:
    // Overrides from Algorithm
    void execute(DataStructure* ds) override;
    void executeAndDisplay(DataStructure* ds) override;
    std::string getName() const override;

    // Setters for parameters
    void setIsSmallest(bool choice);
    void setK(int k);

    // Helper for tests/UI to inspect the last computed result
    int getResult() const;
};
