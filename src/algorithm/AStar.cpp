#include "../../include/algorithm/AStar.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace {
struct NodeEntry {
    int vertex;
    double fScore;

    bool operator>(const NodeEntry& other) const {
        return fScore > other.fScore;
    }
};
} // namespace

void AStar::setStart(int start) {
    startVertex = start;
}

void AStar::setGoal(int goal) {
    goalVertex = goal;
}

const std::vector<int>& AStar::getPath() const {
    return path;
}

double AStar::getPathCost() const {
    return totalCost;
}

void AStar::execute(DataStructure* ds) {
    auto* graph = dynamic_cast<GraphStructure*>(ds);
    path.clear();
    totalCost = 0.0;

    if (!graph) {
        return;
    }

    run(graph);
}

void AStar::executeAndDisplay(DataStructure* ds) {
    execute(ds);
    if (path.empty()) {
        std::cout << "No path found for the provided start/goal vertices." << std::endl;
        return;
    }

    std::cout << "A* path: ";
    display(path);
    std::cout << "Total cost: " << totalCost << std::endl;
}

void AStar::display(const std::vector<int>& elements) {
    for (std::size_t i = 0; i < elements.size(); ++i) {
        std::cout << elements[i];
        if (i + 1 < elements.size()) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
}

void AStar::run(GraphStructure* graph) {
    if (startVertex == -1 || goalVertex == -1) {
        return;
    }
    if (!graph->hasVertex(startVertex) || !graph->hasVertex(goalVertex)) {
        return;
    }

    const auto& adjacency = graph->getAdjacency();
    const double inf = std::numeric_limits<double>::infinity();

    std::unordered_map<int, double> gScore;
    std::unordered_map<int, double> fScore;
    std::unordered_map<int, int> cameFrom;

    for (const auto& [vertex, _] : adjacency) {
        gScore[vertex] = inf;
        fScore[vertex] = inf;
    }

    gScore[startVertex] = 0.0;
    fScore[startVertex] = graph->getHeuristic(startVertex);

    std::priority_queue<NodeEntry, std::vector<NodeEntry>, std::greater<NodeEntry>> openSet;
    openSet.push({startVertex, fScore[startVertex]});

    std::unordered_set<int> closedSet;

    while (!openSet.empty()) {
        NodeEntry currentEntry = openSet.top();
        openSet.pop();
        int current = currentEntry.vertex;

        if (current == goalVertex) {
            totalCost = gScore[current];
            reconstructPath(goalVertex, cameFrom);
            return;
        }

        if (closedSet.find(current) != closedSet.end()) {
            continue;
        }
        closedSet.insert(current);

        auto neighborsIt = adjacency.find(current);
        if (neighborsIt == adjacency.end()) {
            continue;
        }

        for (const auto& [neighbor, weight] : neighborsIt->second) {
            double tentativeG = gScore[current] + weight;
            if (tentativeG < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeG;
                double heuristic = graph->getHeuristic(neighbor);
                double f = tentativeG + heuristic;
                fScore[neighbor] = f;
                openSet.push({neighbor, f});
            }
        }
    }

    // No path found
    path.clear();
    totalCost = inf;
}

void AStar::reconstructPath(int goal, const std::unordered_map<int, int>& cameFrom) {
    path.clear();
    path.push_back(goal);

    auto it = cameFrom.find(goal);
    while (it != cameFrom.end()) {
        path.push_back(it->second);
        it = cameFrom.find(it->second);
    }

    std::reverse(path.begin(), path.end());
}
