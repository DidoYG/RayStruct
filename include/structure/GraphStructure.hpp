// include/structure/GraphStructure.hpp
#pragma once
#include "../model/DataStructure.hpp"
#include <unordered_map>
#include <vector>
#include <utility>

class GraphStructure : public DataStructure {
public:
    using Neighbor = std::pair<int, double>;
    using AdjacencyList = std::unordered_map<int, std::vector<Neighbor>>;

    void insert(int value) override;  // adds a vertex if it does not exist
    void remove(int value) override;  // removes a vertex and its incident edges
    std::vector<int> getElements() const override;  // returns vertex ids

    bool hasVertex(int vertex) const;
    bool addEdge(int from, int to, double weight, bool bidirectional = true);
    bool removeEdge(int from, int to, bool bidirectional = true);
    const AdjacencyList& getAdjacency() const;

    void clear();

    void setHeuristic(int vertex, double value);
    bool hasHeuristic(int vertex) const;
    double getHeuristic(int vertex) const;
    const std::unordered_map<int, double>& getHeuristics() const;

private:
    AdjacencyList adjacency;
    std::unordered_map<int, double> heuristics;
};
