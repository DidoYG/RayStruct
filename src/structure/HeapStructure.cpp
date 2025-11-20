// src/structure/HeapStructure.cpp
#include "../../include/structure/HeapStructure.hpp"

// Implementation of inherited HeapStructure methods
void HeapStructure::insert(int value) {
    data.push_back(value);
}

// Removes the first occurrence of the value from the heap
void HeapStructure::remove(int value) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == value) {
            data.erase(data.begin() + i);
            return;
        }
    }
}

// Retrieves all elements in the heap
std::vector<int> HeapStructure::getElements() const {
    return data;
}

// Get the name of the data structure
std::string HeapStructure::getName() const {
    return "Heap";
}