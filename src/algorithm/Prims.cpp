// src/algorithm/Prims.cpp
#include "../../include/algorithm/Prims.hpp"
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
#include <unordered_set>

// Helper structures and functions for Prim's algorithm
namespace {
// Priority queue entry: (edge weight, from vertex, to vertex)
using QueueEntry = std::tuple<double, int, int>; // weight, from, to

// Comparator for the priority queue (min-heap based on edge weight)
struct QueueCompare {
    bool operator()(const QueueEntry& lhs, const QueueEntry& rhs) const {
        return std::get<0>(lhs) > std::get<0>(rhs);
    }
};
} // namespace

// Prim's helper methods
void Prims::setStart(int start) {
    startVertex = start;
}

const std::vector<std::tuple<int, int, double>>& Prims::getMST() const {
    return mstEdges;
}

double Prims::getTotalWeight() const {
    return totalWeight;
}

bool Prims::isDisconnected() const {
    return isDisconnectedFlag;
}

// Execute Prim's algorithm
void Prims::execute(DataStructure* ds) {
    auto* graph = dynamic_cast<GraphStructure*>(ds);
    mstEdges.clear();
    totalWeight = 0.0;
    isDisconnectedFlag = false;

    if (!graph) {
        return;
    }

    run(graph);
}

// Execute and display results
void Prims::executeAndDisplay(DataStructure* ds) {
    execute(ds);
    if (mstEdges.empty()) {
        std::cout << "No MST generated. Ensure the graph has vertices and edges." << std::endl;
        return;
    }

    std::cout << "Prim's MST edges:\n";
    for (const auto& [from, to, weight] : mstEdges) {
        std::cout << from << " -- " << to << " (w=" << weight << ")\n";
    }
    std::cout << "Total weight: " << totalWeight << std::endl;
    if (isDisconnectedFlag) {
        std::cout << "Note: The graph is disconnected, MST covers only accessible components." << std::endl;
    }
}

// Display elements (vertex ids)
void Prims::display(const std::vector<int>& elements) {
    for (std::size_t i = 0; i < elements.size(); ++i) {
        std::cout << elements[i];
        if (i + 1 < elements.size()) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

// Return the name of the algorithm
std::string Prims::getName() const {
    return "Prim's MST";
}

// Main Prim's algorithm logic
void Prims::run(GraphStructure* graph) {
    // Clear previous results
    const auto& adjacency = graph->getAdjacency();
    if (adjacency.empty()) {
        return;
    }

    // Min-heap priority queue for edges
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, QueueCompare> pq;
    std::unordered_set<int> visited;

    // Lambda to seed the starting vertex
    auto seedStart = [this, graph, &adjacency, &visited, &pq]() {
        int start = startVertex;
        if (!graph->hasVertex(start)) {
            start = adjacency.begin()->first;
        }
        visited.insert(start);
        for (const auto& [neighbor, weight] : adjacency.at(start)) {
            pq.emplace(weight, start, neighbor);
        }
    };

    // Initialize the algorithm
    seedStart();

    // Lambda to enqueue neighbors of a vertex
    auto enqueueNeighbors = [&adjacency, &visited, &pq](int vertex) {
        auto it = adjacency.find(vertex);
        if (it == adjacency.end()) {
            return;
        }
        for (const auto& [neighbor, weight] : it->second) {
            if (visited.find(neighbor) == visited.end()) {
                pq.emplace(weight, vertex, neighbor);
            }
        }
    };

    // Main loop of Prim's algorithm
    while (!pq.empty()) {
        // Get the edge with the smallest weight
        auto [weight, from, to] = pq.top();
        pq.pop();

        // Skip if the destination vertex is already visited
        if (visited.find(to) != visited.end()) {
            continue;
        }

        // Include this edge in the MST
        visited.insert(to);
        mstEdges.emplace_back(from, to, weight);
        totalWeight += weight;

        // Enqueue neighbors of the newly visited vertex
        enqueueNeighbors(to);
    }

    // Check for disconnected components
    if (visited.size() < adjacency.size()) {
        isDisconnectedFlag = true;
        // Process remaining disconnected components
        for (const auto& [vertex, _] : adjacency) {
            // If vertex is unvisited, start a new MST component
            if (visited.find(vertex) == visited.end()) {
                visited.insert(vertex);
                enqueueNeighbors(vertex);
                // Continue Prim's algorithm for this component
                while (!pq.empty()) {
                    // Get the edge with the smallest weight
                    auto [weight, from, to] = pq.top();
                    pq.pop();
                    if (visited.find(to) != visited.end()) {
                        continue;
                    }
                    // Include this edge in the MST
                    visited.insert(to);
                    mstEdges.emplace_back(from, to, weight);
                    totalWeight += weight;
                    enqueueNeighbors(to);
                }
            }
        }
    }
}
