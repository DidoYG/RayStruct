// src/algorithm/InsertionSort.cpp
#include "../../include/algorithm/InsertionSort.hpp"
#include <iostream>
#include <vector>

// Displays the elements of the vector
void InsertionSort::display(const std::vector<int>& elements) {
    for (const int& val : elements) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

// Executes the insertion sort algorithm on the data structure
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

// Executes the insertion sort algorithm and displays each step
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