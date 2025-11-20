⚡️ RayStruct++

RayStruct++ is a modern C++17 command-line benchmarking tool for evaluating and comparing the performance of classical algorithms and data structures.
It allows users to run built-in benchmarks, measure execution time and memory usage, and even upload and execute custom algorithm implementations dynamically.

🧩 Overview

RayStruct++ was built to serve as both an educational platform for students learning algorithms and a research tool for developers experimenting with new data structures or algorithmic optimizations.

It provides an intuitive CLI interface, reliable benchmarking metrics, and support for custom user-defined modules, all built with clean software engineering principles such as Factory and Strategy design patterns.

🚀 Features

📊 Benchmark Classical Algorithms & Structures
| Data Structure | Supported Algorithms                                               |
| -------------- | ------------------------------------------------------------------ |
| **List**       | Insertion Sort, Merge Sort                                         |
| **Heap**       | Build Heap (Min/Max), Heap-Based Selection (k-th smallest/largest) |
| **Graph**      | A* Pathfinding, Prim’s Minimum Spanning Tree (MST)                 |

⚙️ Accurate Performance Measurement

⏱ Execution Time – measured using the <chrono> high-resolution clock.

🧠 Memory Usage – calculated via system-level monitoring (RSS on Linux, Working Set on Windows).

📈 Optional step-by-step display for smaller datasets.

🧩 Modular Architecture

Factory Pattern – dynamically creates structures and algorithms.

Strategy Pattern – interchangeable algorithm behavior at runtime.

Interface Abstraction – algorithms and structures share common base interfaces.

🧠 Custom Implementations

Users can upload their own .cpp or .hpp files, which the system:

Validates and compiles dynamically.

Loads as shared libraries

Benchmarks against the predefined evaluation metrics.

For more information on how to make a valid .cpp implementation look at the /custom folder for examples of DataStructure and Algorithm files.

💻 Cross-Platform CLI

Lightweight, text-based interface.

Clear interactive prompts for structure, algorithm, and input selection.

Designed for Linux and Windows (tested with g++).