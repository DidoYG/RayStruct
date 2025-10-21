#include "../../include/structure/GraphStructure.hpp"
#include <algorithm>
#include <limits>

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

bool GraphStructure::hasVertex(int vertex) const {
    return adjacency.find(vertex) != adjacency.end();
}

bool GraphStructure::addEdge(int from, int to, double weight, bool bidirectional) {
    insert(from);
    insert(to);

    auto addNeighbor = [weight](std::vector<Neighbor>& neighbors, int target) {
        auto existing = std::find_if(neighbors.begin(), neighbors.end(),
                                     [target](const Neighbor& edge) { return edge.first == target; });
        if (existing != neighbors.end()) {
            existing->second = weight;
        } else {
            neighbors.emplace_back(target, weight);
        }
    };

    addNeighbor(adjacency[from], to);
    if (bidirectional) {
        addNeighbor(adjacency[to], from);
    }

    return true;
}

bool GraphStructure::removeEdge(int from, int to, bool bidirectional) {
    bool removed = false;

    auto removeNeighbor = [&removed](std::vector<Neighbor>& neighbors, int target) {
        auto it = std::remove_if(neighbors.begin(), neighbors.end(),
                                 [target](const Neighbor& edge) { return edge.first == target; });
        if (it != neighbors.end()) {
            neighbors.erase(it, neighbors.end());
            removed = true;
        }
    };

    if (auto it = adjacency.find(from); it != adjacency.end()) {
        removeNeighbor(it->second, to);
    }
    if (bidirectional) {
        if (auto it = adjacency.find(to); it != adjacency.end()) {
            removeNeighbor(it->second, from);
        }
    }

    return removed;
}

const GraphStructure::AdjacencyList& GraphStructure::getAdjacency() const {
    return adjacency;
}

void GraphStructure::clear() {
    adjacency.clear();
    heuristics.clear();
}

void GraphStructure::setHeuristic(int vertex, double value) {
    insert(vertex);
    heuristics[vertex] = value;
}

bool GraphStructure::hasHeuristic(int vertex) const {
    return heuristics.find(vertex) != heuristics.end();
}

double GraphStructure::getHeuristic(int vertex) const {
    auto it = heuristics.find(vertex);
    if (it != heuristics.end()) {
        return it->second;
    }
    return 0.0;
}

const std::unordered_map<int, double>& GraphStructure::getHeuristics() const {
    return heuristics;
}
