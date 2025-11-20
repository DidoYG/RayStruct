// tests/TestFramework.hpp
#pragma once
#include <cmath>
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// Simple exception used to report assertion failures
class TestFailure : public std::runtime_error {
public:
    explicit TestFailure(const std::string& message)
        : std::runtime_error(message) {}
};

// Lightweight assertion helper used inside each test case
class TestContext {
public:
    void expect(bool condition, const std::string& message) {
        if (!condition) {
            throw TestFailure(message);
        }
    }

    template <typename T, typename U>
    void expectEqual(const T& actual, const U& expected, const std::string& label) {
        if (!(actual == expected)) {
            std::ostringstream oss;
            oss << label << " (expected: " << expected << ", actual: " << actual << ")";
            throw TestFailure(oss.str());
        }
    }

    template <typename T>
    void expectSequenceEqual(const std::vector<T>& actual,
                             const std::vector<T>& expected,
                             const std::string& label) {
        if (actual.size() != expected.size()) {
            std::ostringstream oss;
            oss << label << " (expected size: " << expected.size()
                << ", actual size: " << actual.size() << ")";
            throw TestFailure(oss.str());
        }
        for (std::size_t i = 0; i < actual.size(); ++i) {
            if (!(actual[i] == expected[i])) {
                std::ostringstream oss;
                oss << label << " (mismatch at index " << i << ": expected " << expected[i]
                    << ", actual " << actual[i] << ")";
                throw TestFailure(oss.str());
            }
        }
    }

    void expectNear(double actual, double expected, double tolerance, const std::string& label) {
        if (std::fabs(actual - expected) > tolerance) {
            std::ostringstream oss;
            oss << label << " (expected: " << expected << ", actual: " << actual
                << ", tolerance: " << tolerance << ")";
            throw TestFailure(oss.str());
        }
    }

    [[noreturn]] void fail(const std::string& message) {
        throw TestFailure(message);
    }
};

// Minimal test suite runner. Each test is a function that receives a TestContext.
class TestSuite {
public:
    using TestFunc = std::function<void(TestContext&)>;

    void add(std::string name, TestFunc func) {
        tests.emplace_back(std::move(name), std::move(func));
    }

    int run() const {
        int passed = 0;
        std::cout << "Running " << tests.size() << " tests...\n";
        for (const auto& [name, func] : tests) {
            TestContext ctx;
            try {
                func(ctx);
                ++passed;
                std::cout << "[PASS] " << name << "\n";
            } catch (const TestFailure& failure) {
                std::cout << "[FAIL] " << name << " -- " << failure.what() << "\n";
            } catch (const std::exception& ex) {
                std::cout << "[FAIL] " << name << " -- unexpected exception: " << ex.what() << "\n";
            } catch (...) {
                std::cout << "[FAIL] " << name << " -- unknown exception\n";
            }
        }
        std::cout << "Passed " << passed << " / " << tests.size() << " tests.\n";
        return passed == static_cast<int>(tests.size()) ? 0 : 1;
    }

private:
    std::vector<std::pair<std::string, TestFunc>> tests;
};
