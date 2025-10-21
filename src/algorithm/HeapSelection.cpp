// src/algorithm/HeapSelection.cpp
#include "../../include/algorithm/HeapSelection.hpp"
#include <iostream>
#include <queue>
#include <functional>

void HeapSelection::display(const std::vector<int>& elements) {
    if (isSmallest) {
        std::cout << "The " << k << "-th smallest element is: " << result << std::endl;
    } else {
        std::cout << "The " << k << "-th largest element is: " << result << std::endl;
    }
}

int HeapSelection::findKthSmallest(std::vector<int>& elements, int k) {
    // max heap to store k smallest
    std::priority_queue<int> maxHeap;

    for (int val : elements) {
        maxHeap.push(val);
        if ((int)maxHeap.size() > k)
            maxHeap.pop(); // keep only k smallest
    }

    return maxHeap.top();
}

int HeapSelection::findKthLargest(std::vector<int>& elements, int k) {
    // min heap to store k largest
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    for (int val : elements) {
        minHeap.push(val);
        if ((int)minHeap.size() > k)
            minHeap.pop(); // keep only k largest
    }

    return minHeap.top();
}

void HeapSelection::execute(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();

    if (isSmallest) {
        findKthSmallest(elements, k);
    } else {
        findKthLargest(elements, k);
    }
}

void HeapSelection::executeAndDisplay(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();

    if (isSmallest) {
        result = findKthSmallest(elements, k);
    } else {
        result = findKthLargest(elements, k);
    }

    display(elements);
}

void HeapSelection::setIsSmallest(bool choice) {
    isSmallest = choice;
}

void HeapSelection::setK(int k) {
    this->k = k;
}
