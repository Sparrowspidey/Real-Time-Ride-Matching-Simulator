# Performance Analysis

## 1. Overview

The performance evaluation measures the effect of OpenMP parallelism on
the real-time ride-matching workload.

The experiment uses 1000 drivers, 10000 riders, 50 simulation ticks,
and a request probability of 0.10.

The parallel matching implementation was evaluated using 1, 2, 4, 6,
8, 10, and 12 OpenMP threads.

---

## 2. Thread Scaling

The measured results are:

| Threads | Total Matching Time (ms) | Average Time/Tick (ms) | Speedup | Efficiency |
|---:|---:|---:|---:|---:|
| 1 | 2376.1042 | 47.5221 | 1.00× | 100.00% |
| 2 | 2424.1247 | 48.4825 | 0.98× | 49.01% |
| 4 | 1324.8974 | 26.4979 | 1.79× | 44.84% |
| 6 | 860.6054 | 17.2121 | 2.76× | 46.02% |
| 8 | 826.8214 | 16.5364 | 2.87× | 35.92% |
| 10 | 696.5554 | 13.9311 | 3.41× | 34.11% |
| 12 | 657.4485 | 13.1490 | 3.61× | 30.12% |

---

## 3. Effect of Thread Count

The results demonstrate a general improvement in matching performance
as the number of OpenMP threads increases.

The one-thread configuration requires 47.5221 ms per tick.

At 4 threads, the average matching time decreases to 26.4979 ms/tick.

At 6 threads, it decreases further to 17.2121 ms/tick.

At 8 threads, the average reaches 16.5364 ms/tick.

At 10 threads, the average decreases to 13.9311 ms/tick.

The best measured configuration is 12 threads, with an average matching
time of 13.1490 ms/tick.

---

## 4. Speedup

The maximum observed speedup is 3.61× at 12 threads.

Compared with the one-thread parallel baseline:

- Baseline: 47.5221 ms/tick
- 12 threads: 13.1490 ms/tick
- Speedup: 3.61×

The corresponding reduction in average matching time is approximately
72.33%.

The speedup is therefore substantial but not linear with thread count.

For example, increasing from 1 to 12 threads does not produce a 12×
speedup. This indicates that the workload contains overhead and
non-parallel portions that limit scalability.

---

## 5. Parallel Efficiency

Parallel efficiency decreases as the thread count increases.

The measured efficiency is:

- 1 thread: 100.00%
- 2 threads: 49.01%
- 4 threads: 44.84%
- 6 threads: 46.02%
- 8 threads: 35.92%
- 10 threads: 34.11%
- 12 threads: 30.12%

The decrease indicates diminishing returns from additional threads.

Although 12 threads provide the best absolute performance, the
parallel work is not scaling proportionally with the number of
processors/threads.

---

## 6. The 2-Thread Result

An interesting observation is that the 2-thread configuration is
slightly slower than the one-thread parallel configuration.

Measured values:

- 1 thread: 47.5221 ms/tick
- 2 threads: 48.4825 ms/tick

This results in a measured speedup of only 0.98×.

This behavior demonstrates that parallel execution introduces overhead.
For a small degree of parallelism, the cost of creating, scheduling,
synchronizing, and coordinating parallel work can temporarily outweigh
the benefit of additional threads.

---

## 7. Diminishing Returns

The performance improvement becomes progressively smaller at higher
thread counts.

The improvement from 1 to 4 threads is significant.

The improvement continues through 6 and 8 threads, while the additional
gain from 8 to 10 and 10 to 12 threads is smaller.

This indicates that the workload is benefiting from parallel execution
but is approaching a practical scaling limit for the tested system.

---

## 8. Best Configuration

The best observed configuration is:

| Metric | Result |
|---|---:|
| Threads | 12 |
| Total matching time | 657.4485 ms |
| Average matching time/tick | 13.1490 ms |
| Speedup | 3.61× |
| Efficiency | 30.12% |
| Matched rides | 7912 |

Based on the measured results, 12 threads provide the lowest matching
time among the tested configurations.

---

## 9. Correctness Across Thread Counts

Every tested parallel configuration produced:

`7912 matched rides`

This includes:

- 1 thread
- 2 threads
- 4 threads
- 6 threads
- 8 threads
- 10 threads
- 12 threads

Therefore, the measured thread-scaling experiment did not show a change
in the matched-ride count when the OpenMP thread count was changed.

This is an important observation because the purpose of the parallel
implementation is to improve execution performance without changing
the expected matching result.

---

## 10. Serial vs Parallel

A separate benchmark compared the serial implementation with the
OpenMP implementation.

### Serial

- Matched rides: 7306
- Total matching time: 5890.157 ms
- Average matching time: 117.803 ms/tick

### Parallel

- Matched rides: 7912
- Total matching time: 4515.611 ms
- Average matching time: 90.3122 ms/tick
- Threads: 10

The parallel implementation has a lower measured matching time in this
comparison.

However, the matched-ride counts differ between the two configurations.
Therefore, this result should not be presented as a pure performance
comparison with identical output.

The thread-scaling experiment is the more appropriate measurement for
evaluating OpenMP scalability because all tested parallel configurations
produced the same matched-ride count.

---

## 11. Overall Findings

The experiment demonstrates that OpenMP parallelization can
substantially reduce the computational cost of ride matching.

The best measured configuration used 12 threads and achieved:

- 13.1490 ms average matching time per tick
- 657.4485 ms total matching time
- 3.61× speedup relative to the one-thread parallel baseline
- 30.12% parallel efficiency

The results also demonstrate that increasing the thread count does not
produce linear speedup.

The reduction in efficiency at higher thread counts indicates the
presence of parallelization overhead and limits to available
parallelism.

Overall, the implementation benefits from multi-threaded execution,
with 12 threads producing the best measured performance among the
tested configurations.