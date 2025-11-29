// src/algorithm/InsertionSort.cpp
#include "../../include/algorithm/InsertionSort.hpp"
#include <iostream>
#include <vector>

// Prints the array so users can observe the intermediate ordering.
void InsertionSort::display(const std::vector<int>& elements) {
    for (const int& val : elements) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

// Runs the insertion sort without printing results.
void InsertionSort::execute(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();
    // Insertion Sort Algorithm
    for (int i = 1; i < elements.size(); ++i) {
        int key = elements[i];
        int j = i - 1;
        while (j >= 0 && elements[j] > key) {
            elements[j + 1] = elements[j];
            --j;
        }
        elements[j + 1] = key;
    }
}

// Performs the same algorithm but shows the array after each operation.
void InsertionSort::executeAndDisplay(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();
    
    display(elements);
    
    // Insertion Sort Algorithm with display
    for (int i = 1; i < elements.size(); ++i) {
        int key = elements[i];
        int j = i - 1;
        while (j >= 0 && elements[j] > key) {
            elements[j + 1] = elements[j];
            --j;

            display(elements); // Display after each swap
        }
        elements[j + 1] = key;
        display(elements); // Display after placing the key
    }
}

// Returns the algorithm's name.
std::string InsertionSort::getName() const {
    return "Insertion Sort";
}
