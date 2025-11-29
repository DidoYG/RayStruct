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

    // Adds a vertex if it does not exist yet.
    void insert(int value) override;
    // Removes a vertex and any incident edges/heuristics.
    void remove(int value) override;
    // Returns the vertex ids for display/benchmarking.
    std::vector<int> getElements() const override;
    // Return the structure's name.
    std::string getName() const override;

    // Graph-specific operations
    // Checks whether the vertex already exists in the adjacency map.
    bool hasVertex(int vertex) const;
    // Adds or updates an edge. It has also an optional feature for both directions.
    bool addEdge(int from, int to, double weight, bool bidirectional = true);
    // Removes edges between the specified vertices.
    bool removeEdge(int from, int to, bool bidirectional = true);
    // Provides read-only adjacency access for algorithms.
    const AdjacencyList& getAdjacency() const;
    // Clears all vertices, edges, and heuristics.
    void clear();

    // Heuristic management
    // Stores the heuristic value for the given vertex.
    void setHeuristic(int vertex, double value);
    // Indicates whether a heuristic value has been provided.
    bool hasHeuristic(int vertex) const;
    // Returns the heuristic value or zero when absent.
    double getHeuristic(int vertex) const;
    // Gives algorithms direct access to all heuristic assignments.
    const std::unordered_map<int, double>& getHeuristics() const;

private:
    // The graph is represented using an adjacency list
    AdjacencyList adjacency;
    std::unordered_map<int, double> heuristics;
};
