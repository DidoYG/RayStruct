// include/algorithm/HeapBuild.hpp
#pragma once
#include "../model/Algorithm.hpp"

class HeapBuild : public Algorithm {
protected:
    bool isMinHeap;

    // Helper functions for heap operations
    void heapify(std::vector<int>& elements, int n, int i, bool isMinHeap);
    void buildHeap(std::vector<int>& elements, bool isMinHeap);
    void buildHeapVisual(std::vector<int>& elements, bool isMinHeap);
    void display(const std::vector<int>& elements) override;

public:
    // Overrides from Algorithm
    void execute(DataStructure* ds) override;
    void executeAndDisplay(DataStructure* ds) override;
    
    // Setter for heap type
    void setHeapType(bool type);
};
