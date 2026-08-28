# Real-Time Ride Matching Simulator

A C++17 real-time ride matching simulator that models drivers, riders, ride requests, nearest-driver matching, ride execution, and OpenMP-based parallel processing.

The project focuses on studying and demonstrating **parallel computing and performance optimization** in a realistic ride-hailing workload.

It provides both **serial and OpenMP-parallel matching implementations**, configurable simulation parameters, automated benchmarking, and performance visualization.

---

## Overview

The simulator models a simplified ride-hailing system operating over a sequence of simulation ticks.

During each tick:

1. Ride requests are generated.
2. Requested rides are matched with available drivers.
3. The nearest idle driver is selected.
4. Matched drivers and riders are assigned appropriate states.
5. Ride execution is simulated.
6. Matching performance is measured.

The computationally intensive driver-search stage can be executed using either:

- Serial processing
- OpenMP parallel processing

The project then measures how the workload scales as the number of OpenMP threads increases.

---

## Main Objectives

The main objectives of the project are:

- Implement a real-time ride-matching simulation.
- Model drivers, riders, rides, positions, and ride execution.
- Implement nearest-driver matching.
- Provide both serial and parallel matching algorithms.
- Use OpenMP to parallelize the computationally intensive matching stage.
- Avoid race conditions during parallel execution.
- Measure matching performance.
- Evaluate thread scalability.
- Calculate speedup and parallel efficiency.
- Generate performance visualizations.
- Provide reproducible benchmark results.

---

## Features

- C++17 implementation
- OpenMP parallelization
- Serial matching implementation
- Nearest-driver matching
- Configurable number of drivers
- Configurable number of riders
- Configurable simulation ticks
- Configurable ride-request probability
- Configurable OpenMP thread count
- JSON-based configuration
- CSV benchmark output
- Automated thread-scaling benchmark
- Speedup calculation
- Parallel-efficiency calculation
- Matplotlib performance visualization
- Benchmark analysis report
- CMake build system
- nlohmann/json for configuration parsing

---

# Project Architecture

The project is organized into several components.

```text
Real-Time-Ride-Matching-Simulator/
│
├── CMakeLists.txt
├── README.md
├── .gitignore
│
├── config/
│   └── config.json
│
├── data/
│
├── external/
│   └── nlohmann_json/
│
├── include/
│   ├── City.hpp
│   ├── Config.hpp
│   ├── Driver.hpp
│   ├── Geo.hpp
│   ├── Matching.hpp
│   ├── Position.hpp
│   ├── Ride.hpp
│   ├── RideExecution.hpp
│   ├── Rider.hpp
│   ├── Simulation.hpp
│   └── Types.hpp
│
├── src/
│   ├── City.cpp
│   ├── Config.cpp
│   ├── Driver.cpp
│   ├── Geo.cpp
│   ├── main.cpp
│   ├── Matching.cpp
│   ├── Ride.cpp
│   ├── RideExecution.cpp
│   ├── Rider.cpp
│   └── Simulation.cpp
│
├── results/
│   ├── benchmark.csv
│   ├── thread_benchmark.csv
│   └── benchmark_analysis.txt
│
└── visualization/
    ├── benchmark_analysis.py
    ├── run_thread_benchmark.py
    ├── thread_scaling.py
    ├── thread_scaling.png
    ├── thread_speedup.png
    └── thread_efficiency.png


System Components
Driver

Represents a driver participating in the simulation.

A driver has:

Driver ID
Position
Current status
Ride assignment information

Drivers can transition between states such as:

IDLE
BUSY
Rider

Represents a passenger requesting a ride.

A rider contains:

Rider ID
Pickup position
Destination position
Current status
Ride

Represents a ride request connecting a rider with a driver.

A ride maintains:

Ride ID
Rider ID
Driver ID
Ride status
Matching Engine

The MatchingEngine implements the ride-matching algorithms.

Two matching approaches are provided:

Serial Matching
OpenMP Parallel Matching

The matching engine searches for the nearest available driver for each requested ride.

Simulation

The Simulation class controls the complete simulation lifecycle.

It manages:

Driver initialization
Rider initialization
Ride-request generation
Simulation ticks
Ride matching
Ride execution
Performance measurement
Benchmark result generation
Configuration

Simulation parameters are loaded from:

config/config.json

Current configuration:

{
    "drivers": 1000,
    "riders": 10000,
    "ticks": 50,
    "request_probability": 0.10,
    "matching_mode": "parallel",
    "threads": 10
}
Configuration Parameters
Parameter	Description
drivers	Number of drivers in the simulation
riders	Number of riders in the simulation
ticks	Number of simulation ticks
request_probability	Probability of generating a ride request
matching_mode	Matching implementation: serial or parallel
threads	Number of OpenMP threads
Requirements
C++ Requirements

The project requires:

C++17-compatible compiler
CMake 3.16 or newer
OpenMP support

The project has been developed and tested using a GCC-based toolchain with OpenMP support.

Python Requirements

Python is used for:

Automated thread benchmarking
Benchmark analysis
Performance visualization

Install Matplotlib with:

python -m pip install matplotlib
External Dependency

The project uses the header-only JSON library:

nlohmann/json

It is stored locally under:

external/nlohmann_json/

The project uses:

external/nlohmann_json/single_include/nlohmann/json.hpp

for configuration parsing.

Building the Project

Open PowerShell in the project root.

Configure with CMake
cmake -S . -B build
Build
cmake --build build --config Release

The resulting executable is:

build/ride_sim.exe
Running the Simulator

From the project root:

.\build\ride_sim.exe

The simulator reads:

config/config.json

and starts the configured simulation.

The program displays:

Number of drivers
Number of riders
Number of simulation ticks
Matching mode
Number of OpenMP threads
Request probability
Simulation progress
Benchmark results

Example startup output:

========================================
 Real-Time Ride Matching Simulator
========================================
Drivers : 1000
Riders  : 10000
Ticks   : 50
Mode    : OPENMP PARALLEL
Threads : 10
Request probability : 0.1
========================================
Matching Algorithm

The simulator uses nearest-driver matching.

For every requested ride, the matching engine searches for an idle driver and selects the driver with the smallest distance from the rider's pickup location.

The distance comparison is performed using squared Euclidean distance:

distance² = dx² + dy²

This avoids repeatedly calculating square roots during the driver search.

The actual square root is calculated only for the selected driver when the distance is required for output.

Serial Matching

The serial implementation processes requested rides one at a time.

Conceptually:

for each requested ride:

    find rider

    search all idle drivers

    calculate distance

    select nearest driver

    assign driver to ride

    update rider status

    update ride status

The driver search is performed sequentially.

This provides the baseline implementation used for comparison.

OpenMP Parallel Matching

The parallel implementation uses a two-phase approach.

Phase 1 — Parallel Candidate Selection

Each requested ride independently searches for its nearest currently-idle driver.

This stage is parallelized using OpenMP:

#pragma omp parallel for

Each iteration calculates a candidate driver for one ride.

The phase does not modify:

Driver state
Rider state
Ride state

Instead, it stores candidate information in temporary arrays.

This significantly reduces the risk of data races.

Phase 2 — Conflict Resolution

Multiple rides can select the same driver during the parallel candidate-selection phase.

Therefore, the selected drivers must be resolved sequentially.

The second phase:

Processes rides in order.
Checks whether the candidate driver is still idle.
Assigns the driver if available.
Marks the driver as busy.
Marks the rider as matched.
Marks the ride as matched.

This preserves deterministic conflict resolution and avoids race conditions.

Parallelization Strategy

The expensive part of the matching algorithm is the nearest-driver search.

For a ride:

search through all drivers

When many rides are requested, this produces a large amount of independent computation.

The project therefore parallelizes the independent search operations.

Conceptually:

                  Ride Requests
                       │
                       ▼
             ┌───────────────────┐
             │ Parallel Search   │
             │                   │
             │ Thread 1 → Ride A │
             │ Thread 2 → Ride B │
             │ Thread 3 → Ride C │
             │ ...               │
             └───────────────────┘
                       │
                       ▼
             Candidate Drivers
                       │
                       ▼
             Sequential Conflict
                 Resolution
                       │
                       ▼
                Final Matches

This approach allows the computationally expensive search stage to benefit from multiple CPU threads while keeping shared-state updates controlled.

Benchmarking

The project includes an automated thread-scaling benchmark.

Run:

python .\visualization\run_thread_benchmark.py

The benchmark evaluates:

1 thread
2 threads
4 threads
6 threads
8 threads
10 threads
12 threads

For each configuration it records:

Thread count
Total matching time
Average matching time per tick
Matched rides

Results are saved to:

results/thread_benchmark.csv
Thread Scaling Results

The latest benchmark produced:

Threads	Average Matching Time / Tick	Speedup	Parallel Efficiency
1	47.5221 ms	1.00×	100.00%
2	48.4825 ms	0.98×	49.01%
4	26.4979 ms	1.79×	44.84%
6	17.2121 ms	2.76×	46.02%
8	16.5364 ms	2.87×	35.92%
10	13.9311 ms	3.41×	34.11%
12	13.1490 ms	3.61×	30.12%

The best observed configuration was:

12 OpenMP threads

with:

Average matching time: 13.1490 ms/tick
Speedup

Speedup is calculated relative to the one-thread baseline.

The formula is:

Speedup = T1 / Tp

where:

T1 = execution time using one thread
Tp = execution time using p threads

The benchmark produced a maximum observed speedup of:

3.61×

at 12 threads.

The average matching time decreased from:

47.5221 ms/tick

to:

13.1490 ms/tick

This corresponds to approximately:

72.33% reduction

in average matching time.

Parallel Efficiency

Parallel efficiency is calculated as:

Efficiency = Speedup / Number of Threads × 100

At 12 threads:

Efficiency = 3.61 / 12 × 100
           ≈ 30.12%

The decrease in efficiency at higher thread counts is expected in a workload containing:

Sequential work
Conflict resolution
OpenMP scheduling overhead
Thread-management overhead
Shared memory access
Synchronization costs

Therefore, increasing the number of threads does not produce perfectly linear speedup.

Benchmark Consistency

The thread-scaling benchmark produced the same matched-ride count for all tested thread configurations:

Matched rides: 7912

This indicates that the parallel implementation produced consistent simulation output across the tested OpenMP thread counts.

Serial vs Parallel Benchmark

A separate comparison between the serial and parallel implementations was performed.

Serial
Matched rides       : 7306
Total matching time : 5890.16 ms
Average match/tick  : 117.803 ms
Parallel
Matched rides       : 7912
Total matching time : 4515.61 ms
Average match/tick  : 90.3122 ms

The parallel implementation achieved a lower average matching time for the tested configuration.

The matched-ride count differs between these separate serial and parallel runs because the two executions can have different matching behavior during the simulation.

The dedicated thread-scaling benchmark focuses on the parallel implementation and verifies consistency across thread counts.

Benchmark Output

The primary benchmark result is stored in:

results/benchmark.csv

Example:

drivers,riders,ticks,mode,threads,matched_rides,total_matching_time_ms,average_matching_time_ms
1000,10000,50,parallel,10,7912,4515.610000,90.312200

Thread-scaling results are stored in:

results/thread_benchmark.csv

Example:

threads,total_matching_time_ms,average_matching_time_ms,matched_rides
1,2376.10,47.5221,7912
2,2424.12,48.4825,7912
4,1324.90,26.4979,7912
6,860.605,17.2121,7912
8,826.821,16.5364,7912
10,696.555,13.9311,7912
12,657.448,13.1490,7912
Benchmark Analysis

The project includes an analysis script:

python .\visualization\benchmark_analysis.py

It calculates and reports:

Baseline performance
Best thread count
Best average matching time
Best total matching time
Maximum speedup
Time improvement
Thread efficiency
Matched-ride consistency

The generated report is saved to:

results/benchmark_analysis.txt

Example summary:

REAL-TIME RIDE MATCHING SIMULATOR
BENCHMARK ANALYSIS
==================================================

Baseline threads              : 1
Baseline average time/tick    : 47.5221 ms

Best thread count             : 12
Best average time/tick        : 13.1490 ms
Best total matching time      : 657.4480 ms

Maximum speedup               : 3.61x
Time improvement              : 72.33%

Matched rides consistent      : YES
Matched rides                 : 7912
Visualization

The project includes Python scripts for visualizing the benchmark results.

Run:

python .\visualization\thread_scaling.py

The script generates:

visualization/thread_scaling.png
visualization/thread_speedup.png
visualization/thread_efficiency.png
Thread Scaling

The thread-scaling graph shows average matching time per tick as the number of OpenMP threads increases.

The observed trend shows a substantial reduction in execution time as the thread count increases, with the best observed result at 12 threads.

Speedup

The speedup graph compares each configuration against the one-thread baseline.

The maximum observed speedup is:

3.61×

at:

12 threads
Parallel Efficiency

The parallel-efficiency graph shows how effectively additional threads contribute to performance.

Efficiency decreases as thread count increases because the application contains both parallel and sequential work.

Reproducible Workflow

A complete build and benchmark workflow is:

1. Configure
cmake -S . -B build
2. Build
cmake --build build --config Release
3. Run the simulator
.\build\ride_sim.exe
4. Run thread benchmark
python .\visualization\run_thread_benchmark.py
5. Generate benchmark analysis
python .\visualization\benchmark_analysis.py
6. Generate visualizations
python .\visualization\thread_scaling.py
Performance Summary

For the tested workload:

Drivers : 1000
Riders  : 10000
Ticks   : 50

the best observed configuration was:

OpenMP threads        : 12
Average matching time : 13.1490 ms/tick
Maximum speedup       : 3.61×
Time reduction        : 72.33%
Parallel efficiency   : 30.12%
Matched rides         : 7912

The results demonstrate that OpenMP parallelization can significantly reduce the computational cost of the ride-matching workload.

However, the results also demonstrate that parallel performance does not scale linearly with thread count.

Performance Interpretation

The benchmark demonstrates several important characteristics of parallel computing.

1. Parallelization improves performance

The matching workload contains a large amount of independent driver-search computation.

Parallelizing this workload reduces average matching time significantly.

2. More threads do not always mean better performance

The two-thread configuration performed slightly worse than the one-thread configuration in the observed benchmark:

1 thread : 47.5221 ms/tick
2 threads: 48.4825 ms/tick

This demonstrates that parallel overhead can exceed the benefits of parallel execution when the workload is not sufficiently large or when system scheduling effects dominate.

3. Performance improves significantly at higher thread counts

Performance improves considerably from 2 through 12 threads.

The best measured result was:

12 threads → 13.1490 ms/tick
4. Speedup is sub-linear

Ideal 12-thread execution would theoretically approach 12× speedup.

The observed speedup was:

3.61×

The difference is caused by:

Sequential conflict resolution
OpenMP overhead
Memory access limitations
Synchronization
Shared data structures
Other serial portions of the simulation
5. Efficiency decreases at higher thread counts

At 12 threads the measured parallel efficiency was:

30.12%

This is an important result because it shows the practical limitations of parallel scalability.

Complexity

For each requested ride, the matching algorithm may inspect every driver.

Therefore, the core nearest-driver search has approximately:

O(R × D)

work per matching operation, where:

R = number of requested rides
D = number of drivers

The project reduces additional lookup overhead by mapping rider IDs directly to vector indices.

Therefore, rider lookup is effectively:

O(1)

The OpenMP implementation distributes the independent ride-search operations across multiple threads.

Race Condition Avoidance

Parallel matching requires careful handling of shared state.

A naive implementation could allow two threads to assign the same driver simultaneously.

The project avoids this by separating the algorithm into two phases.

Phase 1

Threads only calculate candidate drivers.

Shared simulation state is not modified.

Phase 2

Candidate assignments are resolved sequentially.

This ensures that a driver can only be assigned once during conflict resolution.

The approach provides a balance between:

Parallel computation
Deterministic state updates
Race-condition avoidance
Technologies Used
Technology	Purpose
C++17	Core simulator implementation
OpenMP	Parallel ride matching
CMake	Build system
nlohmann/json	JSON configuration parsing
Python	Benchmark automation and analysis
Matplotlib	Performance visualization
Directory Responsibilities
include/

Contains C++ header files and class interfaces.

src/

Contains C++ implementations and the main application entry point.

config/

Contains simulation configuration.

data/

Reserved for simulation data and future datasets.

external/

Contains third-party dependencies.

results/

Contains benchmark CSV files and analysis reports.

visualization/

Contains Python benchmark/visualization scripts and generated plots.

Future Improvements

Possible future improvements include:

More realistic city/map modeling
Larger synthetic workloads
More sophisticated driver-selection algorithms
Spatial indexing for faster nearest-driver queries
KD-tree or grid-based spatial partitioning
More detailed ride lifecycle modeling
Additional performance metrics
Memory-usage analysis
CPU utilization analysis
Multiple benchmark repetitions and statistical aggregation
Automated benchmark reporting
Additional OpenMP scheduling strategies
Comparison with other parallelization approaches
Conclusion

This project demonstrates the application of parallel computing techniques to a real-time ride-matching problem.

The implementation provides both serial and OpenMP-parallel matching algorithms and evaluates their performance using reproducible benchmark workloads.

For the tested workload of 1000 drivers, 10000 riders, and 50 simulation ticks, the best observed configuration used 12 OpenMP threads and achieved:

3.61× speedup
72.33% reduction in average matching time
13.1490 ms average matching time per tick
30.12% parallel efficiency

The project demonstrates that parallelization can provide substantial performance improvements while also highlighting the practical limitations caused by sequential work, synchronization, memory access, and parallel overhead.

Author

Real-Time Ride Matching Simulator

Built as a parallel-computing and performance-analysis project using C++, OpenMP, CMake, Python, and Matplotlib.    