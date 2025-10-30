// include/structure/ListStructure.h
#pragma once
#include "../model/DataStructure.hpp"

class ListStructure : public DataStructure {
private:
    // Internal storage for list elements
    std::vector<int> data;

public:
    // Overrides from DataStructure
    void insert(int value) override;
    void remove(int value) override;
    std::vector<int> getElements() const override;
};
