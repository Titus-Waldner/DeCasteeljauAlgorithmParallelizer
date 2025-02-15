# DeCasteeljauAlgorithmParallelizer

De Casteljau Algorithm Bezier Curve Generator Parallelizer
Overview

This project demonstrates the parallelization of the De Casteljau algorithm for generating Bezier curves. The project includes three implementations, each providing a different level of functionality, parallelization, and execution behavior.
Compatibility

    Windows Only: The provided implementations use #include <windows.h> for mouse movement and pixel detection, making them incompatible with WSL.

Provided Implementations
1. Benzier.cpp (Basic Implementation)

    A simple, uncommented implementation demonstrating the basic De Casteljau algorithm for smooth mouse movement.

    Hardcoded to move to one location.

    Usage: Run Benzier.exe by double-clicking the executable or rebuilding the code in VS Code if necessary.

2. openmpBenzier.cpp (Parallelized Version with OpenMP)

    Allows customization of num_processors, timeBenzier, and schedule_type.

    timeBenzier determines how many times the path is subdivided, affecting runtime.

    Usage:

        Drag the openmpBenzier.exe into a Windows terminal.

        Provide parameters: <num_processors> <timeBenzier> <schedule_type>.

    Notes:

        The third parameter, schedule_type, accepts: dynamic, guided, or static.

        Large timeBenzier values may cause long execution times. Use Ctrl + C to exit.

3. BenzierSimulator.cpp (Simulation Without Mouse Movement)

    Identical to openmpBenzier.cpp but does not move the mouse.

    Useful for performance testing without external system interaction.

De Casteljau Algorithm

A Bezier curve is a parametric curve used in computer graphics for smooth shape generation. The De Casteljau algorithm computes curve points by recursively interpolating between control points using a parameter t (ranging from 0 to 1).

    More details: Bezier Curve Visualization

Parallelization Approach

    Since each t value computation is independent, parallelization is straightforward.

    This project computes the Bezier curve's intermediate points in parallel using OpenMP.

OpenMP Scheduling Options

The project supports three OpenMP scheduling strategies:

    Static: Divides loop iterations into equal-sized chunks distributed among threads.

    Dynamic: Assigns chunks dynamically to threads as they become available.

    Guided: Begins with large chunks and progressively reduces chunk size, balancing load distribution.

Performance Analysis

Performance tests were conducted across 1-core, 4-core, and 8-core configurations using different schedulers. The results confirm:

    Parallelization significantly improves execution times.

    Static and Guided schedulers outperform Dynamic at larger iteration counts due to reduced overhead.

    Speed-up is not linear with core count, indicating hardware and scheduling limitations.

Additional Features
Threadprivate Directive Example (threadprivate_example.cpp)

    Demonstrates the #pragma omp threadprivate directive, which provides each thread with its own persistent copy of a global variable.

    Threads maintain independent values across parallel regions.

OpenMP Declare SIMD Example (declaresimd_example.cpp)

    Uses #pragma omp declare simd to enable SIMD vectorization of a function.

    Demonstrates element-wise addition of arrays using OpenMP's SIMD capabilities.

Compilation Instructions

To compile any of the provided .cpp files with OpenMP support:
 g++ filename.cpp -o output_name -fopenmp -luser32
Running Executables

Simply drag the executable into a Windows terminal and specify required parameters.

This project showcases efficient parallel computation of Bezier curves and highlights OpenMP features for performance optimization in computational geometry tasks.


