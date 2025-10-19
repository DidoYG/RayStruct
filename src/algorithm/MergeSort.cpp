// src/algorithm/MergeSort.cpp
#include "../../include/algorithm/MergeSort.hpp"
#include <iostream>
#include <vector>

void MergeSort::mergeSort(std::vector<int>& elements, int left, int right) {
    if (left >= right) {
        return;
    }
    
    int mid = left + (right - left) / 2;
    mergeSort(elements, left, mid);
    mergeSort(elements, mid + 1, right);
    merge(elements, left, mid, right);
}

// Merge sort with visual output
void MergeSort::mergeSortVisual(std::vector<int>& elements, int left, int right) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;
    mergeSortVisual(elements, left, mid);
    mergeSortVisual(elements, mid + 1, right);
    mergeVisual(elements, left, mid, right);
}

void MergeSort::merge(std::vector<int>& elements, int left, int mid, int right) {
    std::vector<int> leftVec(elements.begin() + left, elements.begin() + mid + 1);
    std::vector<int> rightVec(elements.begin() + mid + 1, elements.begin() + right + 1);

    int i = 0, j = 0, k = left;

    while (i < leftVec.size() && j < rightVec.size()) {
        if (leftVec[i] <= rightVec[j])
            elements[k++] = leftVec[i++];
        else
            elements[k++] = rightVec[j++];
    }

    while (i < leftVec.size())
        elements[k++] = leftVec[i++];

    while (j < rightVec.size())
        elements[k++] = rightVec[j++];
}

// Merge step that shows progress
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

void MergeSort::display(const std::vector<int>& elements) {
    for (const int& val : elements) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void MergeSort::execute(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();
    mergeSort(elements, 0, elements.size()- 1);
}

void MergeSort::executeAndDisplay(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();

    display(elements);
    mergeSortVisual(elements, 0, static_cast<int>(elements.size()) - 1);
    display(elements);
}