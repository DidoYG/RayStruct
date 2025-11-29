// src/structure/HeapStructure.cpp
#include "../../include/structure/HeapStructure.hpp"

// Appends the value so algorithms can later heapify or reorder as needed.
void HeapStructure::insert(int value) {
    data.push_back(value);
}

// Removes the first matching value, keeping other entries untouched.
void HeapStructure::remove(int value) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == value) {
            data.erase(data.begin() + i);
            return;
        }
    }
}

// Returns a copy of the elements for benchmarking output.
std::vector<int> HeapStructure::getElements() const {
    return data;
}

// Returns structure's name.
std::string HeapStructure::getName() const {
    return "Heap";
}
