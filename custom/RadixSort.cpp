#include "../include/model/Algorithm.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

class RadixSort : public Algorithm {
public:
    // Runs the radix sort silently so benchmarks only capture timing.
    void execute(DataStructure* ds) override {
        auto elements = ds->getElements();
        radixSort(elements);
    }

    // Provides before/after visualization of the radix sort.
    void executeAndDisplay(DataStructure* ds) override {
        auto elements = ds->getElements();
        std::cout << "Before sorting: ";
        display(elements);
        radixSort(elements);
        std::cout << "After sorting: ";
        display(elements);
    }

    // Prints elements in a single line for readability.
    void display(const std::vector<int>& elements) override {
        for (int num : elements) {
            std::cout << num << ' ';
        }
        std::cout << std::endl;
    }

    // Prints the algorithm's name after being called by the benchmarking method.
    std::string getName() const override {
        return "Radix Sort";
    }

private:
    // Executes LSD radix sort by iterating through decimal digit positions.
    void radixSort(std::vector<int>& arr) {
        if (arr.empty()) return;
        int maxVal = *std::max_element(arr.begin(), arr.end());
        for (int exp = 1; maxVal / exp > 0; exp *= 10) {
            countingSort(arr, exp);
        }
    }

    // Stable counting sort used for each digit pass of radix sort.
    void countingSort(std::vector<int>& arr, int exp) {
        const int n = static_cast<int>(arr.size());
        std::vector<int> output(n);
        int count[10] = {0};

        for (int value : arr) {
            count[(value / exp) % 10]++;
        }
        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }
        for (int i = n - 1; i >= 0; --i) {
            int digit = (arr[i] / exp) % 10;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }
        for (int i = 0; i < n; ++i) {
            arr[i] = output[i];
        }
    }
};

// Exported factory entry point expected by AlgorithmFactory
// Allows RayStruct++ to instantiate this custom algorithm via dlopen.
extern "C" Algorithm* createAlgorithm() {
    return new RadixSort();
}
