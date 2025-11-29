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

// Comparison for the priority queue (min-heap based on edge weight)
struct QueueCompare {
    bool operator()(const QueueEntry& lhs, const QueueEntry& rhs) const {
        return std::get<0>(lhs) > std::get<0>(rhs);
    }
};
} // namespace

// Sets the user-selected starting vertex.
void Prims::setStart(int start) {
    startVertex = start;
}

// Provides read-only access to the edges collected for the MST.
const std::vector<std::tuple<int, int, double>>& Prims::getMST() const {
    return mstEdges;
}

// Returns the total weight of the MST or of the partial tree if disconnected.
double Prims::getTotalWeight() const {
    return totalWeight;
}

// Signals when the graph had unreachable vertices and the MST is incomplete.
bool Prims::isDisconnected() const {
    return isDisconnectedFlag;
}

// The override of the execute method called by the benchmarking logic.
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

// Runs the algorithm and prints a user-friendly summary of the resulting MST.
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

// Displaying of the vertex lists.
void Prims::display(const std::vector<int>& elements) {
    for (std::size_t i = 0; i < elements.size(); ++i) {
        std::cout << elements[i];
        if (i + 1 < elements.size()) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

// Return's the algorithm's name.
std::string Prims::getName() const {
    return "Prim's MST";
}

// Implements the actual Prim's algorithm with support for disconnected components.
void Prims::run(GraphStructure* graph) {
    // Clear previous results
    const auto& adjacency = graph->getAdjacency();
    if (adjacency.empty()) {
        return;
    }

    // Min-heap priority queue for edges
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, QueueCompare> pq;
    std::unordered_set<int> visited;

    // Lambda function to set the starting vertex
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
