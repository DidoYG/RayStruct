// include/algorithm/MergeSort.hpp
#pragma once
#include "../model/Algorithm.hpp"

class MergeSort : public Algorithm {
private:
    void mergeSort(std::vector<int>& elements, int left, int right);
    void mergeSortVisual(std::vector<int>& elements, int left, int right);
    void merge(std::vector<int>& elements, int left, int mid, int right);
    void mergeVisual(std::vector<int>& elements, int left, int mid, int right);
    void display(const std::vector<int>& elements) override;
    
public:
    void execute(DataStructure* ds) override;
    void executeAndDisplay(DataStructure* ds) override;
};