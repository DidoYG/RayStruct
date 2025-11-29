// src/algorithm/MergeSort.cpp
#include "../../include/algorithm/MergeSort.hpp"
#include <iostream>
#include <vector>

// Performs the recursive divide-and-conquer merge sort.
void MergeSort::mergeSort(std::vector<int>& elements, int left, int right) {
    if (left >= right) {
        return;
    }

    // Recursively split the array
    int mid = left + (right - left) / 2;
    mergeSort(elements, left, mid);
    mergeSort(elements, mid + 1, right);
    merge(elements, left, mid, right);
}

// Identical to mergeSort but prints array states during merging for teaching.
void MergeSort::mergeSortVisual(std::vector<int>& elements, int left, int right) {
    if (left >= right) {
        return;
    }

    // Recursively split the array, but with visual merges
    int mid = left + (right - left) / 2;
    mergeSortVisual(elements, left, mid);
    mergeSortVisual(elements, mid + 1, right);
    mergeVisual(elements, left, mid, right);
}

// Merges the sorted halves back into the main array without extra output.
void MergeSort::merge(std::vector<int>& elements, int left, int mid, int right) {
    // Create temporary vectors for left and right subarrays
    std::vector<int> leftVec(elements.begin() + left, elements.begin() + mid + 1);
    std::vector<int> rightVec(elements.begin() + mid + 1, elements.begin() + right + 1);

    // Merge back into original array
    int i = 0, j = 0, k = left;

    // Merge the temp arrays back into elements[left..right]
    while (i < leftVec.size() && j < rightVec.size()) {
        if (leftVec[i] <= rightVec[j])
            elements[k++] = leftVec[i++];
        else
            elements[k++] = rightVec[j++];
    }

    // Copy any remaining elements of leftVec
    while (i < leftVec.size()) {
        elements[k++] = leftVec[i++];
    }

    // Copy any remaining elements of rightVec
    while (j < rightVec.size()) {
        elements[k++] = rightVec[j++];
    }
}

// Merge step variant that calls display after every copy so users see progress.
void MergeSort::mergeVisual(std::vector<int>& elements, int left, int mid, int right) {
    std::vector<int> leftVec(elements.begin() + left, elements.begin() + mid + 1);
    std::vector<int> rightVec(elements.begin() + mid + 1, elements.begin() + right + 1);

    int i = 0, j = 0, k = left;

    while (i < leftVec.size() && j < rightVec.size()) {
        if (leftVec[i] <= rightVec[j])
            elements[k++] = leftVec[i++];
        else
            elements[k++] = rightVec[j++];

        // Display current array state
        display(elements);
    }

    while (i < leftVec.size()) {
        elements[k++] = leftVec[i++];
        display(elements);
    }

    while (j < rightVec.size()) {
        elements[k++] = rightVec[j++];
        display(elements);
    }
}

// Prints the array in order, useful for before/after comparisons.
void MergeSort::display(const std::vector<int>& elements) {
    for (const int& val : elements) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

// Entry point for benchmarking that runs the non-visual merge sort.
void MergeSort::execute(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();
    mergeSort(elements, 0, elements.size()- 1);
}

// Executes the visual variant to show intermediate merging steps.
void MergeSort::executeAndDisplay(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();

    display(elements);
    mergeSortVisual(elements, 0, static_cast<int>(elements.size()) - 1);
    display(elements);
}

// Returns the algorithm's name.
std::string MergeSort::getName() const {
    return "Merge Sort";
}
