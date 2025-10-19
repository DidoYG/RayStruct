// include/structure/ListStructure.h
#pragma once
#include "../model/DataStructure.hpp"

class ListStructure : public DataStructure {
private:
    std::vector<int> data;

public:
    void insert(int value) override;
    void remove(int value) override;

    std::vector<int> getElements() const override;
};
