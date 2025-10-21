#include "../../include/algorithm/Prims.hpp"
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
#include <unordered_set>

namespace {
using QueueEntry = std::tuple<double, int, int>; // weight, from, to

struct QueueCompare {
    bool operator()(const QueueEntry& lhs, const QueueEntry& rhs) const {
        return std::get<0>(lhs) > std::get<0>(rhs);
    }
};
} // namespace

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

void Prims::display(const std::vector<int>& elements) {
    for (std::size_t i = 0; i < elements.size(); ++i) {
        std::cout << elements[i];
        if (i + 1 < elements.size()) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

void Prims::run(GraphStructure* graph) {
    const auto& adjacency = graph->getAdjacency();
    if (adjacency.empty()) {
        return;
    }

    std::priority_queue<QueueEntry, std::vector<QueueEntry>, QueueCompare> pq;
    std::unordered_set<int> visited;

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

    seedStart();

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

    while (!pq.empty()) {
        auto [weight, from, to] = pq.top();
        pq.pop();

        if (visited.find(to) != visited.end()) {
            continue;
        }

        visited.insert(to);
        mstEdges.emplace_back(from, to, weight);
        totalWeight += weight;

        enqueueNeighbors(to);
    }

    if (visited.size() < adjacency.size()) {
        isDisconnectedFlag = true;
        for (const auto& [vertex, _] : adjacency) {
            if (visited.find(vertex) == visited.end()) {
                visited.insert(vertex);
                enqueueNeighbors(vertex);
                while (!pq.empty()) {
                    auto [weight, from, to] = pq.top();
                    pq.pop();
                    if (visited.find(to) != visited.end()) {
                        continue;
                    }
                    visited.insert(to);
                    mstEdges.emplace_back(from, to, weight);
                    totalWeight += weight;
                    enqueueNeighbors(to);
                }
            }
        }
    }
}
