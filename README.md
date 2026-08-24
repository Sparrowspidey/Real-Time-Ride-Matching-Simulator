# Real-Time Ride Matching Simulator 

A parallel, C-based simulation of a real-time ride-matching system (like Uber/Ola), built to explore how **OpenMP** can speed up the core bottleneck of ride-hailing platforms: matching a growing pool of riders to the nearest available drivers, in real time, at scale.

This is a personal project by **Vivek** and **Reshmitha**, and doubles as a course project on parallel computing.

---

##  Problem Statement

Ride-hailing platforms must continuously:
1. Track the live locations of thousands of drivers (idle / busy).
2. Accept incoming ride requests from riders.
3. Match each rider to the best available driver (usually nearest, sometimes weighted by ETA, rating, surge zone, etc.).
4. Update driver/rider state and repeat — every few seconds, at city scale.

Step 3 is naturally parallelizable: computing the distance/ETA from *every* idle driver to *every* pending request is an embarrassingly parallel workload, and matching many riders in a batch can be parallelized too. This project simulates that pipeline and uses **OpenMP** to parallelize the expensive parts, then benchmarks serial vs. parallel performance.

---

##  Goals

- Simulate a city grid with moving drivers and randomly arriving ride requests.
- Implement a **serial baseline** matching engine.
- Implement a **parallel (OpenMP) version** of the same engine.
- Benchmark speedup across thread counts and problem sizes (drivers × riders).
- Visualize/log results (matching time, wait time, driver utilization).

---

##  Parallelization Strategy

| Stage | Description | Parallelized with |
|---|---|---|
| Distance/ETA computation | For each pending ride request, compute distance to every idle driver | `#pragma omp parallel for` (nested or collapsed loop over requests × drivers) |
| Batch matching | Resolve driver↔rider assignments for a tick's worth of requests | Parallel candidate scoring + critical section / reduction for conflict-free assignment |
| Driver position updates | Move all active drivers one simulation step | `#pragma omp parallel for` over the driver array |
| Metrics aggregation | Compute average wait time, utilization, etc. | `#pragma omp parallel for reduction(...)` |

Synchronization points (e.g. preventing two riders from grabbing the same driver) are handled with `omp critical` / atomic updates on driver status flags, kept as narrow as possible to preserve parallel speedup.

---

##  System Overview

```
                ┌────────────────────┐
                │   Simulator Core     │
                │  (tick-based loop)   │
                └─────────┬──────────┘
                          │
        ┌─────────────────┼─────────────────┐
        │                 │                 │
 ┌──────▼─────┐   ┌───────▼───────┐  ┌──────▼──────┐
 │  Driver Pool │   │ Matching Engine│  │  Rider Queue │
 │ (positions,  │◄──┤ (OpenMP-       ├─►│ (pending     │
 │  status)     │   │  parallel)     │  │  requests)   │
 └──────────────┘   └───────────────┘  └──────────────┘
                          │
                  ┌───────▼────────┐
                  │  Metrics/Logger │
                  └────────────────┘
```

---

##  Project Structure

```
Real-Time-Ride-Matching-Simulator/
├── src/
│   ├── main.c              # Entry point, simulation loop
│   ├── simulator.c      # Tick loop, orchestrates driver/rider/matching updates
│   ├── driver.c        # Driver struct, movement, status
│   ├── rider.c         # Rider struct, request generation
│   ├── matching.c       # Serial + OpenMP matching algorithms
│   ├── geo.c           # Distance/ETA helper functions
│   ├── metrics.c        # Wait time, utilization, throughput stats
│   └── utils.c          # RNG, config parsing, timing helpers
├── include/                 # Shared public headers (if split from src)
├── data/
│   └── config.txt          # Simulation parameters (num drivers, riders, grid size, ticks)
├── scripts/
│   ├── build.sh            # Compile serial + parallel binaries
│   ├── run_benchmarks.sh   # Sweep thread counts / problem sizes
│   └── plot_results.py     # Optional: turn benchmark CSVs into charts
├── results/
│   └── benchmarks/         # CSV/log output from benchmark runs
├── tests/
│   └── test_matching.c     # Sanity tests for matching correctness
├── docs/
│   └── design_notes.md     # Deeper design/architecture notes, diagrams
├── Makefile
├── .gitignore
├── LICENSE
└── README.md
```

---

##  Build & Run

```bash
# Clone
git clone https://github.com/<sparrowspidey>/Real-Time-Ride-Matching-Simulator.git
cd Real-Time-Ride-Matching-Simulator

# Build (serial + OpenMP versions)
make

# Run serial baseline
./bin/simulator_serial --drivers 1000 --riders 500 --ticks 200

# Run parallel version with 8 threads
OMP_NUM_THREADS=8 ./bin/simulator_parallel --drivers 1000 --riders 500 --ticks 200
```

Compile flags of note: `-fopenmp -O2` for the parallel build.

---

##  Benchmarking

`scripts/run_benchmarks.sh` sweeps thread counts (1, 2, 4, 8, 16...) across a few driver/rider scales and logs matching-stage time to `results/benchmarks/`. Speedup and efficiency plots go in `results/` or `docs/`.

---

##  Roadmap

- [ ] Serial baseline simulator
- [ ] OpenMP-parallel distance computation
- [ ] OpenMP-parallel batch matching with conflict resolution
- [ ] Benchmark suite (thread count × scale sweep)
- [ ] Surge pricing / weighted matching (stretch goal)
- [ ] Simple visualization of driver/rider grid over time (stretch goal)

---

##  Team

- **Vivek**
- **Reshmitha**

Course project + personal project — parallel computing (OpenMP), C.

---

##  License

MIT (see `LICENSE`) — update if your course requires a different license.
