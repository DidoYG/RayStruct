// include/structure/HeapStructure.h
#pragma once
#include "../model/DataStructure.hpp"

class HeapStructure : public DataStructure {
private:
    // Internal storage for heap elements
    std::vector<int> data;

public:
    // Overrides from DataStructure
    void insert(int value) override;
    void remove(int value) override;
    std::vector<int> getElements() const override;
    std::string getName() const override;
};
