// src/structure/ListStructure.cpp
#include "../../include/structure/ListStructure.hpp"

// Implementation of inherited ListStructure methods
void ListStructure::insert(int value) {
    data.push_back(value);
}

// Removes the first occurrence of the value
void ListStructure::remove(int value) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == value) {
            data.erase(data.begin() + i);
            return;
        }
    }
}

// Returns all elements in the list
std::vector<int> ListStructure::getElements() const {
    return data;
}

// Get the name of the data structure
std::string ListStructure::getName() const {
    return "List";
}