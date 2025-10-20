// src/algorithm/HeapBuild.cpp
#include "../../include/algorithm/HeapBuild.hpp"
#include <iostream>
#include <vector>
#include <cmath>

void HeapBuild::heapify(std::vector<int>& elements, int n, int i, bool isMinHeap) {
    int extreme = i; // smallest (for min) or largest (for max)
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Compare children with parent
    if (isMinHeap) {
        if (left < n && elements[left] < elements[extreme])
            extreme = left;
        if (right < n && elements[right] < elements[extreme])
            extreme = right;
    } else {
        if (left < n && elements[left] > elements[extreme])
            extreme = left;
        if (right < n && elements[right] > elements[extreme])
            extreme = right;
    }

    // If extreme is not the root, swap and heapify subtree
    if (extreme != i) {
        int tmp = elements[i];
        elements[i] = elements[extreme];
        elements[extreme] = tmp;
        heapify(elements, n, extreme, isMinHeap);
    }
}

void HeapBuild::buildHeap(std::vector<int>& elements, bool isMinHeap) {
    int n = elements.size();

    // Start from last non-leaf node and heapify all subtrees
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(elements, n, i, isMinHeap);
    }
}

void HeapBuild::buildHeapVisual(std::vector<int>& elements, bool isMinHeap) {
    int n = elements.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(elements, n, i, isMinHeap);
        display(elements);
    }
}

void HeapBuild::display(const std::vector<int>& elements) {
    int n = elements.size();
    int levels = std::floor(std::log2(n)) + 1;
    int index = 0;

    for (int level = 0; level < levels; ++level) {
        int levelCount = static_cast<int>(std::pow(2, level));

        // spacing for tree-like shape
        int spaces = static_cast<int>(std::pow(2, levels - level));
        for (int s = 0; s < spaces; ++s)
            std::cout << " ";

        for (int j = 0; j < levelCount && index < n; ++j, ++index) {
            std::cout << elements[index] << " ";
            for (int s = 0; s < spaces; ++s)
                std::cout << " ";
        }

        std::cout << "\n\n";
    }
}

void HeapBuild::execute(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();
    buildHeap(elements, isMinHeap);
}

void HeapBuild::executeAndDisplay(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();

    display(elements);
    buildHeapVisual(elements, isMinHeap);
    display(elements);
}

void HeapBuild::setHeapType(bool type) {
    isMinHeap = type;
}
