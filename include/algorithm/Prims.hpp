// include/algorithm/Prims.hpp
#pragma once
#include "../model/Algorithm.hpp"
#include "../structure/GraphStructure.hpp"
#include <tuple>
#include <vector>

class Prims : public Algorithm {
private:
    int startVertex = -1;
    std::vector<std::tuple<int, int, double>> mstEdges;
    double totalWeight = 0.0;
    bool isDisconnectedFlag = false;

    void run(GraphStructure* graph);

public:
    void setStart(int start);
    const std::vector<std::tuple<int, int, double>>& getMST() const;
    double getTotalWeight() const;
    bool isDisconnected() const;

    void execute(DataStructure* ds) override;
    void executeAndDisplay(DataStructure* ds) override;
    void display(const std::vector<int>& elements) override;
};
