// include/algorithm/Prims.hpp
#pragma once
#include "../model/Algorithm.hpp"
#include "../structure/GraphStructure.hpp"
#include <tuple>
#include <vector>

class Prims : public Algorithm {
private:
    // Prim's algorithm specific members
    int startVertex = -1;
    std::vector<std::tuple<int, int, double>> mstEdges;
    double totalWeight = 0.0;
    bool isDisconnectedFlag = false;

    // Internal method to run Prim's algorithm
    void run(GraphStructure* graph);

public:
    // Public helper methods
    void setStart(int start);
    const std::vector<std::tuple<int, int, double>>& getMST() const;
    double getTotalWeight() const;
    bool isDisconnected() const;

    // Overrides from Algorithm
    void execute(DataStructure* ds) override;
    void executeAndDisplay(DataStructure* ds) override;
    void display(const std::vector<int>& elements) override;
};
