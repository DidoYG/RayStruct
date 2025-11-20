#include "../include/model/DataStructure.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

class Array : public DataStructure {
    std::vector<int> elements;

public:
    void insert(int value) override {
        // Always insert at the start (as per interface)
        elements.insert(elements.begin(), value);
    }

    void remove(int value) override {
        // Remove all instances of value
        elements.erase(std::remove(elements.begin(), elements.end(), value), elements.end());
    }

    std::vector<int> getElements() const override {
        return elements;
    }

    std::string getName() const override {
        return "Array";
    }
};

// Factory function for dynamic loading
extern "C" DataStructure* createDataStructure() {
    return new Array();
}
