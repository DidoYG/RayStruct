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
    // Performs the MST construction logic.
    void run(GraphStructure* graph);

public:
    // Public helper methods
    // Specifies the desired start vertex when available.
    void setStart(int start);
    // Returns the collected MST edges for inspection.
    const std::vector<std::tuple<int, int, double>>& getMST() const;
    // Returns the weight of the built MST.
    double getTotalWeight() const;
    // Indicates whether the graph had more than one component.
    bool isDisconnected() const;

    // Overrides from Algorithm
    // Executes Prim's algorithm without printing.
    void execute(DataStructure* ds) override;
    // Executes Prim's algorithm and prints the resulting MST.
    void executeAndDisplay(DataStructure* ds) override;
    // Prints vertex ids to match Algorithm interface.
    void display(const std::vector<int>& elements) override;
    // Returns the algorithm name.
    std::string getName() const override;
};
