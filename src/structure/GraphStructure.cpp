// src/structure/GraphStructure.cpp
#include "../../include/structure/GraphStructure.hpp"
#include <algorithm>
#include <limits>

// Implementation of inherited GraphStructure methods
void GraphStructure::insert(int value) {
    adjacency.try_emplace(value);
}

void GraphStructure::remove(int value) {
    adjacency.erase(value);
    for (auto& [vertex, neighbors] : adjacency) {
        auto it = std::remove_if(neighbors.begin(), neighbors.end(),
                                 [value](const Neighbor& edge) { return edge.first == value; });
        if (it != neighbors.end()) {
            neighbors.erase(it, neighbors.end());
        }
    }
    heuristics.erase(value);
}

std::vector<int> GraphStructure::getElements() const {
    std::vector<int> vertices;
    vertices.reserve(adjacency.size());
    for (const auto& [vertex, _] : adjacency) {
        vertices.push_back(vertex);
    }
    return vertices;
}

// Implementation of graph-specific operations
bool GraphStructure::hasVertex(int vertex) const {
    return adjacency.find(vertex) != adjacency.end();
}

// Adds an edge from 'from' to 'to' with the specified weight
bool GraphStructure::addEdge(int from, int to, double weight, bool bidirectional) {
    insert(from);
    insert(to);

    // Lambda to add or update a neighbor
    auto addNeighbor = [weight](std::vector<Neighbor>& neighbors, int target) {
        auto existing = std::find_if(neighbors.begin(), neighbors.end(),
                                     [target](const Neighbor& edge) { return edge.first == target; });
        if (existing != neighbors.end()) {
            existing->second = weight;
        } else {
            neighbors.emplace_back(target, weight);
        }
    };

    // Add or update the edge from 'from' to 'to'
    addNeighbor(adjacency[from], to);
    // If bidirectional, add or update the edge from 'to' to 'from'
    if (bidirectional) {
        addNeighbor(adjacency[to], from);
    }

    return true;
}

// Removes the edge from 'from' to 'to'
bool GraphStructure::removeEdge(int from, int to, bool bidirectional) {
    bool removed = false;

    // Lambda to remove a neighbor
    auto removeNeighbor = [&removed](std::vector<Neighbor>& neighbors, int target) {
        auto it = std::remove_if(neighbors.begin(), neighbors.end(),
                                 [target](const Neighbor& edge) { return edge.first == target; });
        if (it != neighbors.end()) {
            neighbors.erase(it, neighbors.end());
            removed = true;
        }
    };

    // Remove the edge from 'from' to 'to'
    if (auto it = adjacency.find(from); it != adjacency.end()) {
        removeNeighbor(it->second, to);
    }
    // If bidirectional, remove the edge from 'to' to 'from'
    if (bidirectional) {
        if (auto it = adjacency.find(to); it != adjacency.end()) {
            removeNeighbor(it->second, from);
        }
    }

    return removed;
}

// Returns the adjacency list of the graph
const GraphStructure::AdjacencyList& GraphStructure::getAdjacency() const {
    return adjacency;
}

// Clears the entire graph
void GraphStructure::clear() {
    adjacency.clear();
    heuristics.clear();
}

// Heuristic management implementations
void GraphStructure::setHeuristic(int vertex, double value) {
    insert(vertex);
    heuristics[vertex] = value;
}

// Checks if a heuristic exists for the given vertex
bool GraphStructure::hasHeuristic(int vertex) const {
    return heuristics.find(vertex) != heuristics.end();
}

// Retrieves the heuristic value for the given vertex
double GraphStructure::getHeuristic(int vertex) const {
    auto it = heuristics.find(vertex);
    if (it != heuristics.end()) {
        return it->second;
    }
    return 0.0;
}

// Returns all heuristics in the graph
const std::unordered_map<int, double>& GraphStructure::getHeuristics() const {
    return heuristics;
}
