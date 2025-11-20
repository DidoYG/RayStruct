#include "../include/model/Algorithm.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

class RadixSort : public Algorithm {
public:
    void execute(DataStructure* ds) override {
        auto elements = ds->getElements();
        radixSort(elements);
        // optionally write the sorted values back if the host expects mutation
    }

    void executeAndDisplay(DataStructure* ds) override {
        auto elements = ds->getElements();
        std::cout << "Before sorting: ";
        display(elements);
        radixSort(elements);
        std::cout << "After sorting: ";
        display(elements);
    }

    void display(const std::vector<int>& elements) override {
        for (int num : elements) {
            std::cout << num << ' ';
        }
        std::cout << std::endl;
    }

private:
    void radixSort(std::vector<int>& arr) {
        if (arr.empty()) return;
        int maxVal = *std::max_element(arr.begin(), arr.end());
        for (int exp = 1; maxVal / exp > 0; exp *= 10) {
            countingSort(arr, exp);
        }
    }

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
extern "C" Algorithm* createAlgorithm() {
    return new RadixSort();
}
