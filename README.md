# CSE331 Sorting Algorithms Project

This project implements and benchmarks **12 comparison-based sorting algorithms** for educational and analytical purposes as part of the 2025 CSE331 Assignment #1 at UNIST.

##  Algorithms Implemented

###  Basic Sorting Algorithms
- Bubble Sort
- Insertion Sort
- Selection Sort
- Merge Sort
- Quick Sort (median-of-three pivot, Lomuto partition)
- Heap Sort

###  Advanced / Hybrid Sorting Algorithms
- Library Sort
- Tim Sort
- Cocktail Shaker Sort
- Comb Sort
- Tournament Sort
- Introsort

---

##  Experimental Setup

- **Language**: C++17  
- **Compiler**: `g++ 14.2.0` via MinGW-w64  
- **Optimization**: `-O3`  
- **Platform**: Windows 10  
- **CPU**: AMD Ryzen 5 5600X  
- **RAM**: 32GB

All experiments were repeated 10 times per configuration using `<chrono>` for timing and `std::is_sorted()` for correctness verification.

---

##  Data Types Tested

Each algorithm was benchmarked using the following input patterns:

- **Sorted (Ascending)**
- **Reversed (Descending)**
- **Random**
- **Partially Sorted** (10% shuffled)

---

##  Output

Results are stored in `sorting_results.csv` with the following schema:

