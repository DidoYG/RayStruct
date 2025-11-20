// tests/test_main.cpp
#include "TestFramework.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "../include/algorithm/AStar.hpp"
#include "../include/algorithm/AlgorithmFactory.hpp"
#include "../include/algorithm/HeapBuild.hpp"
#include "../include/algorithm/HeapSelection.hpp"
#include "../include/algorithm/InsertionSort.hpp"
#include "../include/algorithm/MergeSort.hpp"
#include "../include/algorithm/Prims.hpp"
#include "../include/manager/BenchmarkManager.hpp"
#include "../include/model/Algorithm.hpp"
#include "../include/model/DataStructure.hpp"
#include "../include/structure/DataStructureFactory.hpp"
#include "../include/structure/GraphStructure.hpp"
#include "../include/structure/HeapStructure.hpp"
#include "../include/structure/ListStructure.hpp"

// Simple in-memory data structure used to feed deterministic values to algorithms
class VectorDataStructure : public DataStructure {
public:
    explicit VectorDataStructure(std::vector<int> values = {})
        : data(std::move(values)) {}

    void insert(int value) override {
        data.push_back(value);
    }

    void remove(int value) override {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (*it == value) {
                data.erase(it);
                return;
            }
        }
    }

    std::vector<int> getElements() const override {
        return data;
    }

    std::string getName() const override {
        return "VectorDataStructure";
    }

private:
    std::vector<int> data;
};

// Utility for redirecting std::cout to an ostringstream while running an algorithm
class ScopedStreamRedirect {
public:
    ScopedStreamRedirect(std::ostream& stream, std::ostream& replacement)
        : stream(stream), oldBuf(stream.rdbuf(replacement.rdbuf())) {}

    ~ScopedStreamRedirect() {
        stream.rdbuf(oldBuf);
    }

private:
    std::ostream& stream;
    std::streambuf* oldBuf;
};

// Adapter exposing HeapBuild::buildHeap so we can validate heap layouts
class HeapBuildTestAdapter : public HeapBuild {
public:
    std::vector<int> build(std::vector<int> values, bool isMinHeap) {
        buildHeap(values, isMinHeap);
        return values;
    }
};

// Dummy algorithm for exercising BenchmarkManager behavior
class DummyAlgorithm : public Algorithm {
public:
    void execute(DataStructure*) override {
        ++executeCount;
    }

    void executeAndDisplay(DataStructure* ds) override {
        execute(ds);
    }

    void display(const std::vector<int>&) override {}

    std::string getName() const override {
        return "Dummy";
    }

    int getExecuteCount() const {
        return executeCount;
    }

private:
    int executeCount = 0;
};

// Helpers
std::vector<int> extractLastNumberLine(const std::string& text) {
    std::istringstream input(text);
    std::string line;
    std::vector<std::string> numericLines;

    while (std::getline(input, line)) {
        bool hasDigit = false;
        for (char ch : line) {
            if (std::isdigit(static_cast<unsigned char>(ch)) || ch == '-') {
                hasDigit = true;
                break;
            }
        }
        if (hasDigit) {
            numericLines.push_back(line);
        }
    }

    if (numericLines.empty()) {
        return {};
    }

    std::vector<int> values;
    std::istringstream lastLine(numericLines.back());
    int value;
    while (lastLine >> value) {
        values.push_back(value);
    }
    return values;
}

int extractLastInteger(const std::string& text) {
    bool building = false;
    int sign = 1;
    long current = 0;
    int lastValue = 0;
    bool found = false;

    for (char ch : text) {
        if (ch == '-' && !building) {
            building = true;
            sign = -1;
            current = 0;
        } else if (std::isdigit(static_cast<unsigned char>(ch))) {
            if (!building) {
                building = true;
                sign = 1;
                current = 0;
            }
            current = current * 10 + (ch - '0');
        } else if (building) {
            lastValue = static_cast<int>(sign * current);
            found = true;
            building = false;
            sign = 1;
            current = 0;
        }
    }

    if (building) {
        lastValue = static_cast<int>(sign * current);
        found = true;
    }

    if (!found) {
        throw TestFailure("No integer found in the provided output.");
    }

    return lastValue;
}

bool isValidMinHeap(const std::vector<int>& values) {
    for (std::size_t i = 0; i < values.size(); ++i) {
        std::size_t left = 2 * i + 1;
        std::size_t right = 2 * i + 2;
        if (left < values.size() && values[i] > values[left]) {
            return false;
        }
        if (right < values.size() && values[i] > values[right]) {
            return false;
        }
    }
    return true;
}

bool isValidMaxHeap(const std::vector<int>& values) {
    for (std::size_t i = 0; i < values.size(); ++i) {
        std::size_t left = 2 * i + 1;
        std::size_t right = 2 * i + 2;
        if (left < values.size() && values[i] < values[left]) {
            return false;
        }
        if (right < values.size() && values[i] < values[right]) {
            return false;
        }
    }
    return true;
}

double getEdgeWeight(const GraphStructure::AdjacencyList& adjacency, int from, int to) {
    auto it = adjacency.find(from);
    if (it == adjacency.end()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    for (const auto& [neighbor, weight] : it->second) {
        if (neighbor == to) {
            return weight;
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

int main() {
    TestSuite suite;

    suite.add("ListStructure preserves insertion order", [](TestContext& ctx) {
        ListStructure list;
        list.insert(3);
        list.insert(7);
        list.insert(-2);
        ctx.expectSequenceEqual(list.getElements(), std::vector<int>{3, 7, -2},
                                "ListStructure::insert must append values");
    });

    suite.add("ListStructure remove deletes first occurrence", [](TestContext& ctx) {
        ListStructure list;
        list.insert(1);
        list.insert(2);
        list.insert(2);
        list.insert(3);
        list.remove(2);
        ctx.expectSequenceEqual(list.getElements(), std::vector<int>{1, 2, 3},
                                "ListStructure::remove removes first match");
    });

    suite.add("HeapStructure insert/remove mirrors vector behavior", [](TestContext& ctx) {
        HeapStructure heap;
        heap.insert(5);
        heap.insert(1);
        heap.insert(5);
        heap.remove(5);
        ctx.expectSequenceEqual(heap.getElements(), std::vector<int>{1, 5},
                                "HeapStructure should remove only the first matching value");
    });

    suite.add("GraphStructure stores unique vertices", [](TestContext& ctx) {
        GraphStructure graph;
        graph.insert(10);
        graph.insert(20);
        graph.insert(10);
        auto elements = graph.getElements();
        std::sort(elements.begin(), elements.end());
        ctx.expectSequenceEqual(elements, std::vector<int>{10, 20},
                                "GraphStructure must only store unique vertex ids");
    });

    suite.add("GraphStructure addEdge updates weights bidirectionally", [](TestContext& ctx) {
        GraphStructure graph;
        graph.addEdge(1, 2, 5.0);
        graph.addEdge(1, 2, 3.0);
        const auto& adj = graph.getAdjacency();
        ctx.expectNear(getEdgeWeight(adj, 1, 2), 3.0, 1e-9, "Edge weight 1->2 should update");
        ctx.expectNear(getEdgeWeight(adj, 2, 1), 3.0, 1e-9, "Edge weight 2->1 should update");
    });

    suite.add("GraphStructure removeEdge respects direction flag", [](TestContext& ctx) {
        GraphStructure graph;
        graph.addEdge(1, 2, 5.0, false);
        ctx.expect(getEdgeWeight(graph.getAdjacency(), 2, 1) != 5.0,
                   "Directed edge should only exist in one direction");
        ctx.expect(graph.removeEdge(1, 2, false), "Directed edge removal should succeed");
        ctx.expect(std::isnan(getEdgeWeight(graph.getAdjacency(), 1, 2)),
                   "Edge should be removed from origin vertex");
    });

    suite.add("GraphStructure heuristics persist until cleared", [](TestContext& ctx) {
        GraphStructure graph;
        graph.setHeuristic(1, 4.0);
        ctx.expect(graph.hasHeuristic(1), "Heuristic should exist");
        ctx.expectNear(graph.getHeuristic(1), 4.0, 1e-9, "Heuristic value should match");
        graph.clear();
        ctx.expect(!graph.hasHeuristic(1), "clear() should remove heuristics");
        ctx.expect(graph.getAdjacency().empty(), "clear() should remove adjacency entries");
    });

    suite.add("DataStructureFactory creates built-in types", [](TestContext& ctx) {
        std::unique_ptr<DataStructure> list(DataStructureFactory::createDataStructure(DataStructureEnum::LIST));
        std::unique_ptr<DataStructure> heap(DataStructureFactory::createDataStructure(DataStructureEnum::HEAP));
        std::unique_ptr<DataStructure> graph(DataStructureFactory::createDataStructure(DataStructureEnum::GRAPH));
        ctx.expect(dynamic_cast<ListStructure*>(list.get()) != nullptr, "LIST should create ListStructure");
        ctx.expect(dynamic_cast<HeapStructure*>(heap.get()) != nullptr, "HEAP should create HeapStructure");
        ctx.expect(dynamic_cast<GraphStructure*>(graph.get()) != nullptr, "GRAPH should create GraphStructure");
    });

    suite.add("DataStructureFactory returns nullptr for unknown type", [](TestContext& ctx) {
        DataStructure* ds = DataStructureFactory::createDataStructure(DataStructureEnum::UNKNOWN);
        ctx.expect(ds == nullptr, "Unknown data structure enum must return nullptr");
    });

    suite.add("AlgorithmFactory creates built-in algorithms", [](TestContext& ctx) {
        std::unique_ptr<Algorithm> insertion(AlgorithmFactory::createAlgorithm(AlgorithmEnum::INSERTION_SORT));
        std::unique_ptr<Algorithm> merge(AlgorithmFactory::createAlgorithm(AlgorithmEnum::MERGE_SORT));
        std::unique_ptr<Algorithm> heapBuild(AlgorithmFactory::createAlgorithm(AlgorithmEnum::HEAP_BUILD));
        std::unique_ptr<Algorithm> heapSelect(AlgorithmFactory::createAlgorithm(AlgorithmEnum::HEAP_SELECTION));
        std::unique_ptr<Algorithm> astar(AlgorithmFactory::createAlgorithm(AlgorithmEnum::A_STAR));
        std::unique_ptr<Algorithm> prims(AlgorithmFactory::createAlgorithm(AlgorithmEnum::PRIMS));
        ctx.expect(dynamic_cast<InsertionSort*>(insertion.get()) != nullptr, "Factory should return InsertionSort");
        ctx.expect(dynamic_cast<MergeSort*>(merge.get()) != nullptr, "Factory should return MergeSort");
        ctx.expect(dynamic_cast<HeapBuild*>(heapBuild.get()) != nullptr, "Factory should return HeapBuild");
        ctx.expect(dynamic_cast<HeapSelection*>(heapSelect.get()) != nullptr, "Factory should return HeapSelection");
        ctx.expect(dynamic_cast<AStar*>(astar.get()) != nullptr, "Factory should return AStar");
        ctx.expect(dynamic_cast<Prims*>(prims.get()) != nullptr, "Factory should return Prims");
    });

    suite.add("AlgorithmFactory returns nullptr for unknown type", [](TestContext& ctx) {
        Algorithm* algo = AlgorithmFactory::createAlgorithm(AlgorithmEnum::UNKNOWN);
        ctx.expect(algo == nullptr, "Unknown algorithm enum must return nullptr");
    });

    suite.add("InsertionSort outputs sorted order", [](TestContext& ctx) {
        VectorDataStructure ds({5, 1, 4, 2});
        InsertionSort sorter;
        std::ostringstream captured;
        {
            ScopedStreamRedirect redirect(std::cout, captured);
            sorter.executeAndDisplay(&ds);
        }
        auto finalState = extractLastNumberLine(captured.str());
        ctx.expectSequenceEqual(finalState, std::vector<int>({1, 2, 4, 5}),
                                "InsertionSort final state should be sorted");
    });

    suite.add("MergeSort outputs sorted order", [](TestContext& ctx) {
        VectorDataStructure ds({9, 3, 7, 3, 1});
        MergeSort sorter;
        std::ostringstream captured;
        {
            ScopedStreamRedirect redirect(std::cout, captured);
            sorter.executeAndDisplay(&ds);
        }
        auto finalState = extractLastNumberLine(captured.str());
        ctx.expectSequenceEqual(finalState, std::vector<int>({1, 3, 3, 7, 9}),
                                "MergeSort final state should be sorted");
    });

    suite.add("HeapBuild produces valid min heap", [](TestContext& ctx) {
        HeapBuildTestAdapter builder;
        std::vector<int> values = builder.build({9, 1, 6, 3, 4, 8}, true);
        ctx.expect(isValidMinHeap(values), "buildHeap(true) should enforce min-heap property");
    });

    suite.add("HeapBuild produces valid max heap", [](TestContext& ctx) {
        HeapBuildTestAdapter builder;
        std::vector<int> values = builder.build({2, 7, 1, 9, 5, 8}, false);
        ctx.expect(isValidMaxHeap(values), "buildHeap(false) should enforce max-heap property");
    });

    suite.add("HeapSelection finds k-th smallest", [](TestContext& ctx) {
        VectorDataStructure ds({7, 4, 9, 2, 1});
        HeapSelection selection;
        selection.setIsSmallest(true);
        selection.setK(3);
        selection.execute(&ds);
        ctx.expectEqual(selection.getResult(), 4, "3rd smallest in {1,2,4,7,9} is 4");
    });

    suite.add("HeapSelection finds k-th largest", [](TestContext& ctx) {
        VectorDataStructure ds({5, 12, 3, 9, 7});
        HeapSelection selection;
        selection.setIsSmallest(false);
        selection.setK(2);
        selection.execute(&ds);
        ctx.expectEqual(selection.getResult(), 9, "2nd largest in {12,9,7,5,3} is 9");
    });

    suite.add("AStar finds optimal path", [](TestContext& ctx) {
        GraphStructure graph;
        graph.addEdge(1, 2, 1.0);
        graph.addEdge(2, 3, 1.0);
        graph.addEdge(3, 4, 1.0);
        graph.addEdge(1, 4, 10.0);
        graph.setHeuristic(1, 3.0);
        graph.setHeuristic(2, 2.0);
        graph.setHeuristic(3, 1.0);
        graph.setHeuristic(4, 0.0);

        AStar astar;
        astar.setStart(1);
        astar.setGoal(4);
        astar.execute(&graph);

        ctx.expectSequenceEqual(astar.getPath(), std::vector<int>({1, 2, 3, 4}),
                                "A* should favor the low-cost route");
        ctx.expectNear(astar.getPathCost(), 3.0, 1e-9, "A* cost should match accumulated weights");
    });

    suite.add("AStar returns empty path when goal unreachable", [](TestContext& ctx) {
        GraphStructure graph;
        graph.insert(1);
        graph.insert(2);
        graph.insert(3);
        graph.addEdge(1, 2, 1.0);
        graph.setHeuristic(1, 1.0);
        graph.setHeuristic(2, 1.0);
        graph.setHeuristic(3, 0.0);

        AStar astar;
        astar.setStart(1);
        astar.setGoal(3);
        astar.execute(&graph);

        ctx.expect(astar.getPath().empty(), "No path should be returned when goal is unreachable");
        ctx.expect(std::isinf(astar.getPathCost()), "Unreachable goal should set cost to infinity");
    });

    suite.add("Prims builds MST with expected weight", [](TestContext& ctx) {
        GraphStructure graph;
        graph.addEdge(0, 1, 1.0);
        graph.addEdge(1, 2, 2.0);
        graph.addEdge(0, 2, 4.0);

        Prims prims;
        prims.setStart(0);
        prims.execute(&graph);

        ctx.expectEqual(static_cast<int>(prims.getMST().size()), 2,
                        "MST on a 3-node graph should contain 2 edges");
        ctx.expectNear(prims.getTotalWeight(), 3.0, 1e-9, "MST total weight should be optimal");
        ctx.expect(!prims.isDisconnected(), "Connected graph should not set disconnected flag");
    });

    suite.add("Prims flags disconnected graphs", [](TestContext& ctx) {
        GraphStructure graph;
        graph.addEdge(0, 1, 2.0);
        graph.addEdge(2, 3, 5.0);

        Prims prims;
        prims.setStart(0);
        prims.execute(&graph);

        ctx.expect(prims.isDisconnected(), "Graph with multiple components should be flagged");
        ctx.expectEqual(static_cast<int>(prims.getMST().size()), 2,
                        "Disconnected graph should still collect spanning edges per component");
    });

    suite.add("BenchmarkManager invokes algorithm execute", [](TestContext& ctx) {
        BenchmarkManager benchmark;
        VectorDataStructure ds({1, 2, 3});
        DummyAlgorithm algo;
        benchmark.runBenchmark(&ds, &algo);
        ctx.expectEqual(algo.getExecuteCount(), 1, "Benchmark should trigger algorithm exactly once");
    });

    return suite.run();
}
