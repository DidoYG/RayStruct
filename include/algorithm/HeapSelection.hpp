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
    // Prints the result to the console.
    void display(const std::vector<int>& elements) override;
    // Returns the k-th smallest value using a max-heap.
    int findKthSmallest(std::vector<int>& elements, int k);
    // Returns the k-th largest value using a min-heap.
    int findKthLargest(std::vector<int>& elements, int k);

public:
    // Overrides from Algorithm
    // Computes the statistic without printing.
    void execute(DataStructure* ds) override;
    // Computes the statistic and prints the result summary.
    void executeAndDisplay(DataStructure* ds) override;
    // Returns the algorithm name.
    std::string getName() const override;

    // Setters for parameters
    // Chooses smallest (true) versus largest (false) search mode.
    void setIsSmallest(bool choice);
    // Sets which k value to retrieve.
    void setK(int k);

    // Helper for tests/UI to inspect the last computed result
    // Provides access to the previously computed value.
    int getResult() const;
};
