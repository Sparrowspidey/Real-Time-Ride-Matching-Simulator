# Experimental Setup

## 1. Objective

The objective of the experiment is to evaluate the performance of the
Real-Time Ride Matching Simulator under different OpenMP thread counts.

The experiment measures:

- Total matching time
- Average matching time per simulation tick
- Parallel speedup
- Parallel efficiency
- Number of successfully matched rides

The primary goal is to determine how increasing the number of OpenMP
threads affects the performance of the ride-matching algorithm.

---

## 2. Simulation Configuration

The benchmark uses the following simulation configuration:

| Parameter | Value |
|---|---:|
| Drivers | 1000 |
| Riders | 10000 |
| Simulation ticks | 50 |
| Request probability | 0.10 |
| Matching mode | OpenMP parallel |
| Tested threads | 1, 2, 4, 6, 8, 10, 12 |

The configuration is defined in:

`config/config.json`

---

## 3. Independent Variable

The independent variable is the number of OpenMP threads.

The following thread counts were evaluated:

- 1
- 2
- 4
- 6
- 8
- 10
- 12

---

## 4. Dependent Variables

The experiment measures the following dependent variables.

### Average Matching Time per Tick

This measures the average amount of time required by the matching
operation during one simulation tick.

Lower values indicate better performance.

### Total Matching Time

This is the accumulated matching time across all simulation ticks.

Lower values indicate better performance.

### Speedup

Speedup is calculated relative to the one-thread parallel baseline:

`Speedup = T1 / Tp`

where:

- `T1` is the average matching time using one thread.
- `Tp` is the average matching time using `p` threads.

### Parallel Efficiency

Parallel efficiency is calculated as:

`Efficiency = Speedup / p × 100`

where:

- `p` is the number of OpenMP threads.

### Matched Rides

The number of matched rides is recorded to verify that changing the
thread count does not alter the expected result of the parallel
simulation.

---

## 5. Baseline

The one-thread parallel configuration is used as the baseline for the
thread-scaling experiment.

Measured baseline:

- Threads: 1
- Average matching time: 47.5221 ms/tick
- Total matching time: 2376.1042 ms
- Matched rides: 7912

Therefore:

`Speedup(1) = 1.00×`

and:

`Efficiency(1) = 100%`

---

## 6. Thread Scaling Results

| Threads | Avg. Time/Tick (ms) | Speedup | Efficiency |
|---:|---:|---:|---:|
| 1 | 47.5221 | 1.00× | 100.00% |
| 2 | 48.4825 | 0.98× | 49.01% |
| 4 | 26.4979 | 1.79× | 44.84% |
| 6 | 17.2121 | 2.76× | 46.02% |
| 8 | 16.5364 | 2.87× | 35.92% |
| 10 | 13.9311 | 3.41× | 34.11% |
| 12 | 13.1490 | 3.61× | 30.12% |

All tested configurations produced:

`7912 matched rides`

---

## 7. Best Observed Configuration

The best measured configuration used 12 OpenMP threads.

Results:

- Average matching time: 13.1490 ms/tick
- Total matching time: 657.4485 ms
- Speedup: 3.61×
- Parallel efficiency: 30.12%
- Matched rides: 7912

Compared with the one-thread parallel baseline, the 12-thread
configuration reduced average matching time by approximately 72.33%.

---

## 8. Serial vs Parallel Comparison

A separate comparison was performed between the serial matching
implementation and the OpenMP parallel implementation.

### Serial

- Matching mode: Serial
- Threads: 1
- Average matching time: 117.803 ms/tick
- Total matching time: 5890.157 ms
- Matched rides: 7306

### Parallel

- Matching mode: OpenMP parallel
- Threads: 10
- Average matching time: 90.3122 ms/tick
- Total matching time: 4515.611 ms
- Matched rides: 7912

The serial and parallel implementations are therefore treated as
separate experimental configurations.

The thread-scaling experiment instead uses the one-thread parallel
configuration as its performance baseline.

---

## 9. Experimental Interpretation

The measurements show that increasing the number of OpenMP threads
generally reduces the matching time.

The largest improvement occurs between 2 and 8 threads. Additional
threads continue to improve performance, with the best observed result
at 12 threads.

The improvement is not perfectly linear because parallel execution has
overhead and because some parts of the matching process remain
sequential.

The measured efficiency decreases as the number of threads increases,
which indicates diminishing returns from additional parallelism.

---

## 10. Correctness Observation

The thread-scaling benchmark produced the same matched-ride count of
7912 for every tested OpenMP thread configuration.

This provides evidence that the parallel matching implementation
maintains consistent simulation output while changing the level of
parallel execution.

---

## 11. Reproducibility

The benchmark can be reproduced using the project's visualization
benchmark script:

`visualization/run_thread_benchmark.py`

The benchmark results are stored in:

`results/thread_benchmark.csv`

The analysis is generated using:

`visualization/benchmark_analysis.py`

The visualizations are generated using:

`visualization/thread_scaling.py`