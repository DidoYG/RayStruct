// include/structure/ListStructure.h
#pragma once
#include "../model/DataStructure.hpp"

class ListStructure : public DataStructure {
private:
    // Internal storage for list elements
    std::vector<int> data;

public:
    // Overrides from DataStructure
    // Appends new items at the end of the list.
    void insert(int value) override;
    // Deletes the first occurrence of the provided value.
    void remove(int value) override;
    // Returns a copy of the stored elements.
    std::vector<int> getElements() const override;
    // Returns the structure name.
    std::string getName() const override;
};
