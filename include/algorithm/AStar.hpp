// include/algorithm/AStar.hpp
#pragma once
#include "../model/Algorithm.hpp"
#include "../structure/GraphStructure.hpp"
#include <vector>
#include <unordered_map>

class AStar : public Algorithm {
private:
    // A* specific members
    int startVertex = -1;
    int goalVertex = -1;
    std::vector<int> path;
    double totalCost = 0.0;
    
    // Internal methods
    // Core search routine operating on the provided graph.
    void run(GraphStructure* graph);
    // Goes through the parent map to build the final path.
    void reconstructPath(int current, const std::unordered_map<int, int>& cameFrom);

public:
    // Public helper methods
    // Sets the start vertex chosen by the user.
    void setStart(int start);
    // Sets the goal vertex to search for.
    void setGoal(int goal);
    // Returns the path discovered by the latest run.
    const std::vector<int>& getPath() const;
    // Returns the total cost of the latest path.
    double getPathCost() const;

    // Overrides from Algorithm
    // Executes the A* search without printing results.
    void execute(DataStructure* ds) override;
    // Executes the search and prints either the path or an empty result.
    void executeAndDisplay(DataStructure* ds) override;
    // Outputs a path with "->" separators.
    void display(const std::vector<int>& elements) override;
    // Provides the algorithm name for menus/output.
    std::string getName() const override;
};
