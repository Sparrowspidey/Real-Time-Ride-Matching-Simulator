import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RESULTS_FILE = ROOT / "results" / "thread_benchmark.csv"
REPORT_FILE = ROOT / "results" / "benchmark_analysis.txt"

with RESULTS_FILE.open(newline="", encoding="utf-8") as file:
    rows = list(csv.DictReader(file))

if not rows:
    raise RuntimeError("thread_benchmark.csv contains no benchmark data.")

for row in rows:
    row["threads"] = int(row["threads"])
    row["total_matching_time_ms"] = float(row["total_matching_time_ms"])
    row["average_matching_time_ms"] = float(row["average_matching_time_ms"])
    row["matched_rides"] = int(row["matched_rides"])

baseline = next(
    row for row in rows
    if row["threads"] == 1
)

best = min(
    rows,
    key=lambda row: row["average_matching_time_ms"]
)

baseline_time = baseline["average_matching_time_ms"]
best_time = best["average_matching_time_ms"]

speedup = baseline_time / best_time
improvement = (1.0 - best_time / baseline_time) * 100.0

matched_values = {
    row["matched_rides"]
    for row in rows
}

consistent = len(matched_values) == 1

lines = []

lines.append("REAL-TIME RIDE MATCHING SIMULATOR")
lines.append("BENCHMARK ANALYSIS")
lines.append("=" * 50)
lines.append("")
lines.append(f"Baseline threads              : {baseline['threads']}")
lines.append(
    f"Baseline average time/tick    : "
    f"{baseline_time:.4f} ms"
)
lines.append("")
lines.append(f"Best thread count             : {best['threads']}")
lines.append(
    f"Best average time/tick        : "
    f"{best_time:.4f} ms"
)
lines.append(
    f"Best total matching time      : "
    f"{best['total_matching_time_ms']:.4f} ms"
)
lines.append("")
lines.append(f"Maximum speedup               : {speedup:.2f}x")
lines.append(f"Time improvement              : {improvement:.2f}%")
lines.append("")
lines.append(
    f"Matched rides consistent      : "
    f"{'YES' if consistent else 'NO'}"
)

if consistent:
    lines.append(
        f"Matched rides                 : "
        f"{next(iter(matched_values))}"
    )
else:
    lines.append(
        "Matched ride counts           : "
        + ", ".join(str(value) for value in sorted(matched_values))
    )

lines.append("")
lines.append("THREAD SCALING")
lines.append("-" * 50)
lines.append(
    f"{'Threads':>8} "
    f"{'Avg/tick (ms)':>16} "
    f"{'Speedup':>12} "
    f"{'Efficiency':>14}"
)

for row in rows:
    threads = row["threads"]
    avg_time = row["average_matching_time_ms"]

    row_speedup = baseline_time / avg_time
    efficiency = row_speedup / threads * 100.0

    lines.append(
        f"{threads:>8} "
        f"{avg_time:>16.4f} "
        f"{row_speedup:>12.2f}x "
        f"{efficiency:>13.2f}%"
    )

lines.append("")
lines.append("INTERPRETATION")
lines.append("-" * 50)
lines.append(
    f"The best observed configuration used "
    f"{best['threads']} OpenMP threads."
)
lines.append(
    f"It reduced average matching time from "
    f"{baseline_time:.4f} ms/tick to "
    f"{best_time:.4f} ms/tick."
)
lines.append(
    f"This represents a {speedup:.2f}x speedup "
    f"and a {improvement:.2f}% reduction in matching time."
)

if consistent:
    lines.append(
        "All thread configurations produced the same "
        "matched-ride count, indicating consistent simulation output."
    )
else:
    lines.append(
        "Matched-ride counts differ between configurations. "
        "This should be investigated before treating the benchmark "
        "as a strict correctness-equivalent comparison."
    )

REPORT_FILE.write_text(
    "\n".join(lines) + "\n",
    encoding="utf-8"
)

print("\n".join(lines))
print(f"\nReport saved to: {REPORT_FILE}")
