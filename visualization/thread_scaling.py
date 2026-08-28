import csv
from pathlib import Path

import matplotlib.pyplot as plt


ROOT = Path(__file__).resolve().parent.parent

RESULTS_FILE = (
    ROOT / "results" / "thread_benchmark.csv"
)

OUTPUT_DIR = (
    ROOT / "visualization"
)


threads = []
average_times = []

with RESULTS_FILE.open(
    newline="",
    encoding="utf-8"
) as file:

    reader = csv.DictReader(file)

    for row in reader:
        threads.append(
            int(row["threads"])
        )

        average_times.append(
            float(row["average_matching_time_ms"])
        )


if not threads:
    raise RuntimeError(
        "No benchmark data found."
    )


baseline_time = average_times[0]

speedups = [
    baseline_time / time
    for time in average_times
]

efficiencies = [
    (speedup / thread) * 100.0
    for thread, speedup in zip(
        threads,
        speedups
    )
]


# --------------------------------------------------
# 1. THREAD SCALING
# --------------------------------------------------

plt.figure(figsize=(9, 5.5))

plt.plot(
    threads,
    average_times,
    marker="o"
)

plt.xlabel(
    "OpenMP threads"
)

plt.ylabel(
    "Average matching time per tick (ms)"
)

plt.title(
    "OpenMP Thread Scaling"
)

plt.xticks(threads)

plt.grid(
    True,
    alpha=0.3
)

plt.tight_layout()

scaling_file = (
    OUTPUT_DIR / "thread_scaling.png"
)

plt.savefig(
    scaling_file,
    dpi=150
)

plt.close()


# --------------------------------------------------
# 2. SPEEDUP
# --------------------------------------------------

plt.figure(figsize=(9, 5.5))

plt.plot(
    threads,
    speedups,
    marker="o"
)

plt.xlabel(
    "OpenMP threads"
)

plt.ylabel(
    "Speedup"
)

plt.title(
    "OpenMP Speedup"
)

plt.xticks(threads)

plt.grid(
    True,
    alpha=0.3
)

plt.tight_layout()

speedup_file = (
    OUTPUT_DIR / "thread_speedup.png"
)

plt.savefig(
    speedup_file,
    dpi=150
)

plt.close()


# --------------------------------------------------
# 3. PARALLEL EFFICIENCY
# --------------------------------------------------

plt.figure(figsize=(9, 5.5))

plt.plot(
    threads,
    efficiencies,
    marker="o"
)

plt.xlabel(
    "OpenMP threads"
)

plt.ylabel(
    "Parallel efficiency (%)"
)

plt.title(
    "OpenMP Parallel Efficiency"
)

plt.xticks(threads)

plt.grid(
    True,
    alpha=0.3
)

plt.tight_layout()

efficiency_file = (
    OUTPUT_DIR / "thread_efficiency.png"
)

plt.savefig(
    efficiency_file,
    dpi=150
)

plt.close()


# --------------------------------------------------
# SUMMARY
# --------------------------------------------------

best_index = average_times.index(
    min(average_times)
)

print(
    f"Loaded: {RESULTS_FILE}"
)

print(
    f"Saved: {scaling_file}"
)

print(
    f"Saved: {speedup_file}"
)

print(
    f"Saved: {efficiency_file}"
)

print()

print(
    f"Best thread count: "
    f"{threads[best_index]}"
)

print(
    f"Best average time: "
    f"{average_times[best_index]:.4f} ms/tick"
)

print(
    f"Maximum speedup: "
    f"{speedups[best_index]:.2f}x"
)

print(
    f"Parallel efficiency: "
    f"{efficiencies[best_index]:.2f}%"
)
