// src/structure/GraphStructure.cpp
#include "../../include/structure/GraphStructure.hpp"
#include <algorithm>
#include <limits>

// Inserts the vertex into adjacency map if missing so edges can be attached.
void GraphStructure::insert(int value) {
    adjacency.try_emplace(value);
}

// Removes the vertex and strips any existing incoming edges plus heuristic data.
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

// Returns a simple list of vertex ids so algorithms can inspect the graph.
std::vector<int> GraphStructure::getElements() const {
    std::vector<int> vertices;
    vertices.reserve(adjacency.size());
    for (const auto& [vertex, _] : adjacency) {
        vertices.push_back(vertex);
    }
    return vertices;
}

// Provides the display name for benchmarking.
std::string GraphStructure::getName() const {
    return "Graph";
}

// Verifies that a vertex is already tracked in the adjacency list.
bool GraphStructure::hasVertex(int vertex) const {
    return adjacency.find(vertex) != adjacency.end();
}

// Adds or updates an edge between vertices, handling bidirectional edges when requested.
bool GraphStructure::addEdge(int from, int to, double weight, bool bidirectional) {
    insert(from);
    insert(to);

    // Lambda function that adds or updates a neighbor
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

// Removes the edge(s) between two vertices, supporting symmetric removal.
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

// Returns the adjacency list for algorithms that need raw neighbor data.
const GraphStructure::AdjacencyList& GraphStructure::getAdjacency() const {
    return adjacency;
}

// Clears all vertices, edges, and heuristic values.
void GraphStructure::clear() {
    adjacency.clear();
    heuristics.clear();
}

// Assigns or overwrites the heuristic value for a vertex.
void GraphStructure::setHeuristic(int vertex, double value) {
    insert(vertex);
    heuristics[vertex] = value;
}

// Determines whether a heuristic entry is available for the vertex.
bool GraphStructure::hasHeuristic(int vertex) const {
    return heuristics.find(vertex) != heuristics.end();
}

// Looks up the heuristic, returning zero when the vertex lacks an explicit value.
double GraphStructure::getHeuristic(int vertex) const {
    auto it = heuristics.find(vertex);
    if (it != heuristics.end()) {
        return it->second;
    }
    return 0.0;
}

// Gives callers read-only access to every stored heuristic entry.
const std::unordered_map<int, double>& GraphStructure::getHeuristics() const {
    return heuristics;
}
