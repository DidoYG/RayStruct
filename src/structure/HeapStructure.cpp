// src/structure/HeapStructure.cpp
#include "../../include/structure/HeapStructure.hpp"

void HeapStructure::insert(int value) {
    data.push_back(value);
}

void HeapStructure::remove(int value) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == value) {
            data.erase(data.begin() + i);
            return;
        }
    }
}

std::vector<int> HeapStructure::getElements() const {
    return data;
}