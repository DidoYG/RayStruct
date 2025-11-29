// src/algorithm/HeapSelection.cpp
#include "../../include/algorithm/HeapSelection.hpp"
#include <iostream>
#include <queue>
#include <functional>

// Returns the computed k-th statistic and whether it was min or max based.
void HeapSelection::display(const std::vector<int>& elements) {
    if (isSmallest) {
        std::cout << "The " << k << "-th smallest element is: " << result << std::endl;
    } else {
        std::cout << "The " << k << "-th largest element is: " << result << std::endl;
    }
}

// Uses a max-heap to keep the k smallest values seen so far.
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

// Mirrors findKthSmallest but retains the k largest elements through a min-heap.
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

// Computes the requested statistic without printing anything (benchmark mode).
void HeapSelection::execute(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();

    if (isSmallest) {
        result = findKthSmallest(elements, k);
    } else {
        result = findKthLargest(elements, k);
    }
}

// Runs the selection and prints the answer to console.
void HeapSelection::executeAndDisplay(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();

    if (isSmallest) {
        result = findKthSmallest(elements, k);
    } else {
        result = findKthLargest(elements, k);
    }

    display(elements);
}

// Gives the algorithm's name.
std::string HeapSelection::getName() const {
    return "Heap Selection";
}

// Chooses between smallest or largest variants of the search.
void HeapSelection::setIsSmallest(bool choice) {
    isSmallest = choice;
}

// Stores the user-provided value of k.
void HeapSelection::setK(int k) {
    this->k = k;
}

// Getter for the last computed k-th value.
int HeapSelection::getResult() const {
    return result;
}
