// include/algorithm/HeapSelection.hpp
#pragma once
#include "../model/Algorithm.hpp"

class HeapSelection : public Algorithm {
private:
    bool isSmallest;
    int k;
    int result;
    
    int findKthSmallest(std::vector<int>& elements, int k);
    int findKthLargest(std::vector<int>& elements, int k);
    void display(const std::vector<int>& elements) override;

public:
    void execute(DataStructure* ds) override;
    void executeAndDisplay(DataStructure* ds) override;
    void setIsSmallest(bool choice);
    void setK(int k);
};
