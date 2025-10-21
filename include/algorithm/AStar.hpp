// include/algorithm/AStar.hpp
#pragma once
#include "../model/Algorithm.hpp"
#include "../structure/GraphStructure.hpp"
#include <vector>
#include <unordered_map>

class AStar : public Algorithm {
private:
    int startVertex = -1;
    int goalVertex = -1;
    std::vector<int> path;
    double totalCost = 0.0;

    void run(GraphStructure* graph);
    void reconstructPath(int current, const std::unordered_map<int, int>& cameFrom);

public:
    void setStart(int start);
    void setGoal(int goal);
    const std::vector<int>& getPath() const;
    double getPathCost() const;

    void execute(DataStructure* ds) override;
    void executeAndDisplay(DataStructure* ds) override;
    void display(const std::vector<int>& elements) override;
};
