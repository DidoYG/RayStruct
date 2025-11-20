// include/structure/GraphStructure.hpp
#pragma once
#include "../model/DataStructure.hpp"
#include <unordered_map>
#include <vector>
#include <utility>

class GraphStructure : public DataStructure {
public:
    // A neighbor is represented as a pair of (vertex id, edge weight)
    using Neighbor = std::pair<int, double>;

    // The adjacency list maps each vertex id to a list of its neighbors
    using AdjacencyList = std::unordered_map<int, std::vector<Neighbor>>;

    void insert(int value) override;  // adds a vertex if it does not exist
    void remove(int value) override;  // removes a vertex and its incident edges
    std::vector<int> getElements() const override;  // returns a list of vertices
    std::string getName() const override; // returns the name of the data structure

    // Graph-specific operations
    bool hasVertex(int vertex) const;
    bool addEdge(int from, int to, double weight, bool bidirectional = true);
    bool removeEdge(int from, int to, bool bidirectional = true);
    const AdjacencyList& getAdjacency() const;
    void clear();

    // Heuristic management
    void setHeuristic(int vertex, double value);
    bool hasHeuristic(int vertex) const;
    double getHeuristic(int vertex) const;
    const std::unordered_map<int, double>& getHeuristics() const;

private:
    // The graph is represented using an adjacency list
    AdjacencyList adjacency;
    std::unordered_map<int, double> heuristics;
};
