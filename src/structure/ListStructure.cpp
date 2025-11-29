// src/structure/ListStructure.cpp
#include "../../include/structure/ListStructure.hpp"

// Appends an item to the end of the list representation.
void ListStructure::insert(int value) {
    data.push_back(value);
}

// Searches for and deletes the first matching value.
void ListStructure::remove(int value) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == value) {
            data.erase(data.begin() + i);
            return;
        }
    }
}

// Returns the entire list of elements.
std::vector<int> ListStructure::getElements() const {
    return data;
}

// Returns the structure's name for display.
std::string ListStructure::getName() const {
    return "List";
}
