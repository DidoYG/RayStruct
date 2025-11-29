// include/structure/HeapStructure.h
#pragma once
#include "../model/DataStructure.hpp"

class HeapStructure : public DataStructure {
private:
    // Internal storage for heap elements
    std::vector<int> data;

public:
    // Overrides from DataStructure
    // Adds a value to the heap backing vector.
    void insert(int value) override;
    // Removes the first matching value from storage.
    void remove(int value) override;
    // Returns all current elements for algorithm consumption.
    std::vector<int> getElements() const override;
    // Returns the data structure's display name.
    std::string getName() const override;
};
