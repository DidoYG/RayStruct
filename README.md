# RayStruct++

**RayStruct++** is a C++ command-line application designed to **benchmark and compare classical algorithms and data structures**.  
It enables users to execute predefined algorithms on predefined structures, measure their performance (execution time and memory usage), and even upload and test their **own custom implementations**.  

This project serves as both an **educational platform** for understanding algorithmic performance and a **research tool** for testing and analyzing custom solutions.

---

## 🚀 Features

- **Benchmark classical data structures and algorithms**
  - 📋 **List** – Insertion Sort, Merge Sort  
  - 🔺 **Heap** – Build Heap (Min/Max), Heap-Based Selection (k-th smallest/largest element)  
  - 🕸️ **Graph** – A* (A-Star) Pathfinding, Prim’s Minimum Spanning Tree (MST)

- **Accurate performance measurements**
  - Execution time tracking using `<chrono>`
  - Memory usage estimation via system-level monitoring

- **Custom algorithm integration**
  - Users can provide file paths to their own `.cpp` or `.hpp` implementations  
  - The system dynamically integrates and benchmarks the uploaded code

- **CLI-based user interface**
  - Intuitive text-based menus for selecting structures, algorithms, and input sizes  
  - Clear console output with execution results and performance summaries  
---