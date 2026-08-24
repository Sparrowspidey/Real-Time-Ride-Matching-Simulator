# Design Notes — Phase 1

## Matching model

Each pending rider selects the nearest currently idle driver using squared Euclidean distance. Squared distance avoids `sqrt()` because only relative ordering matters.

## Parallel design

1. Parallel region computes one best-driver candidate per rider.
2. A narrow serial conflict-resolution pass commits candidates in rider order.
3. Driver movement is parallelized independently.

This is intentionally conservative for correctness. A later phase can replace the commit pass with a more scalable ownership/reduction strategy.

## Benchmarking

The benchmark measures only the matching stage, not process startup or console output. Compare serial and parallel binaries using identical seeds and parameters.
