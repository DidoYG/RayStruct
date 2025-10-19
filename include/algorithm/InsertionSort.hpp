// include/algorithm/InsertionSort.hpp
#pragma once
#include "../model/Algorithm.hpp"

class InsertionSort : public Algorithm {
private:
    void display(const std::vector<int>& elements) override;
    
public:
    void execute(DataStructure* ds) override;
    void executeAndDisplay(DataStructure* ds) override;
};