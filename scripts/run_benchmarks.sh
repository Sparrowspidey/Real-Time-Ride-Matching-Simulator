#!/usr/bin/env bash
set -euo pipefail
mkdir -p results/benchmarks
OUT="results/benchmarks/benchmark.csv"
echo "threads,drivers,riders,ticks,matching_time_seconds" > "$OUT"

for drivers in 1000 5000 10000; do
  for riders in 500 2500 5000; do
    for threads in 1 2 4 8 16; do
      export OMP_NUM_THREADS="$threads"
      line=$(./bin/simulator_parallel --drivers "$drivers" --riders "$riders" --ticks 50 | grep '^matching_time_seconds=')
      time_value=${line#matching_time_seconds=}
      echo "$threads,$drivers,$riders,50,$time_value" >> "$OUT"
    done
  done
done

echo "Wrote $OUT"
