#include "../include/model/DataStructure.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

class Array : public DataStructure {
    std::vector<int> elements;

public:
    // Inserts each value at the beginning to demonstrate custom logic.
    void insert(int value) override {
        elements.insert(elements.begin(), value);
    }

    // Removes all occurrences of the provided value from the array.
    void remove(int value) override {
        // Remove all instances of value
        elements.erase(std::remove(elements.begin(), elements.end(), value), elements.end());
    }

    // Returns a copy so that the different algorithms can operate on the stored items.
    std::vector<int> getElements() const override {
        return elements;
    }

    // Prints the name of the data structure after being called by the benchmarking method.
    std::string getName() const override {
        return "Array";
    }
};

// Factory function for dynamic loading
// Entry point used by DataStructureFactory when loading this plugin.
extern "C" DataStructure* createDataStructure() {
    return new Array();
}
