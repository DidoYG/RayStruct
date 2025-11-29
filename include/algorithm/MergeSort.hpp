// include/algorithm/MergeSort.hpp
#pragma once
#include "../model/Algorithm.hpp"

class MergeSort : public Algorithm {
private:
    // Helpers for merge sort
    // Standard recursive merge sort implementation.
    void mergeSort(std::vector<int>& elements, int left, int right);
    // Merge sort implementation that prints progress.
    void mergeSortVisual(std::vector<int>& elements, int left, int right);
    // Plain merge step without visual output.
    void merge(std::vector<int>& elements, int left, int mid, int right);
    // Merge step that displays after each write.
    void mergeVisual(std::vector<int>& elements, int left, int mid, int right);
    // Prints the array contents.
    void display(const std::vector<int>& elements) override;
    
public:
    // Overrides from Algorithm
    // Executes merge sort without additional output.
    void execute(DataStructure* ds) override;
    // Executes the visual merge sort variant.
    void executeAndDisplay(DataStructure* ds) override;
    // Returns the algorithm's display name.
    std::string getName() const override;
};
