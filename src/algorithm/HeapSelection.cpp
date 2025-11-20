// src/algorithm/HeapSelection.cpp
#include "../../include/algorithm/HeapSelection.hpp"
#include <iostream>
#include <queue>
#include <functional>

// Displays the result
void HeapSelection::display(const std::vector<int>& elements) {
    if (isSmallest) {
        std::cout << "The " << k << "-th smallest element is: " << result << std::endl;
    } else {
        std::cout << "The " << k << "-th largest element is: " << result << std::endl;
    }
}

// Finds the k-th smallest element using a max heap
int HeapSelection::findKthSmallest(std::vector<int>& elements, int k) {
    // max heap to store k smallest
    std::priority_queue<int> maxHeap;

    // Build the heap with the first k elements
    for (int val : elements) {
        maxHeap.push(val);
        if ((int)maxHeap.size() > k)
            maxHeap.pop(); // keep only k smallest
    }

    return maxHeap.top();
}

// Finds the k-th largest element using a min heap
int HeapSelection::findKthLargest(std::vector<int>& elements, int k) {
    // min heap to store k largest
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    // Build the heap with the first k elements
    for (int val : elements) {
        minHeap.push(val);
        if ((int)minHeap.size() > k)
            minHeap.pop(); // keep only k largest
    }

    return minHeap.top();
}

// Executes the selection algorithm without displaying
void HeapSelection::execute(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();

    if (isSmallest) {
        findKthSmallest(elements, k);
    } else {
        findKthLargest(elements, k);
    }
}

// Executes the selection algorithm and displays the result
void HeapSelection::executeAndDisplay(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();

    if (isSmallest) {
        result = findKthSmallest(elements, k);
    } else {
        result = findKthLargest(elements, k);
    }

    display(elements);
}

// Returns the name of the algorithm
std::string HeapSelection::getName() const {
    return "Heap Selection";
}

// Sets whether to find the k-th smallest or largest element
void HeapSelection::setIsSmallest(bool choice) {
    isSmallest = choice;
}

// Sets the k-th position to find
void HeapSelection::setK(int k) {
    this->k = k;
}
