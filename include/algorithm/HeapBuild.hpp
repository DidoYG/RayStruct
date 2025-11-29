// include/algorithm/HeapBuild.hpp
#pragma once
#include "../model/Algorithm.hpp"

class HeapBuild : public Algorithm {
protected:
    bool isMinHeap;

    // Helper functions for heap operations
    // Restores the heap property for the subtree rooted at index i.
    void heapify(std::vector<int>& elements, int n, int i, bool isMinHeap);
    // Builds a heap without printing intermediate states.
    void buildHeap(std::vector<int>& elements, bool isMinHeap);
    // Builds a heap while displaying each step.
    void buildHeapVisual(std::vector<int>& elements, bool isMinHeap);
    // Renders the array as a tree-like structure.
    void display(const std::vector<int>& elements) override;

public:
    // Overrides from Algorithm
    // Performs the heap build silently.
    void execute(DataStructure* ds) override;
    // Performs the heap build with visual output.
    void executeAndDisplay(DataStructure* ds) override;
    // Returns the algorithm name.
    std::string getName() const override;
    
    // Setter for heap type
    // Chooses between min-heap (true) or max-heap (false).
    void setHeapType(bool type);
};
