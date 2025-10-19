// include/structure/GraphStructure.h
#pragma once
#include "../model/DataStructure.hpp"
#include <unordered_map>

class GraphStructure : public DataStructure {
private:
    std::unordered_map<int, std::vector<int>> adj;

public:
    void insert(int value) override;

    void remove(int value) override;

    std::vector<int> getElements() const override;
};
