// src/algorithm/InsertionSort.cpp
#include "../../include/algorithm/InsertionSort.hpp"
#include <iostream>
#include <vector>

void InsertionSort::display(const std::vector<int>& elements) {
    for (const int& val : elements) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void InsertionSort::execute(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();
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

void InsertionSort::executeAndDisplay(DataStructure* ds) {
    std::vector<int> elements = ds->getElements();
    
    display(elements);

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