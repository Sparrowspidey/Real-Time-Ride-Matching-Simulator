import csv
import sys
from collections import defaultdict

try:
    import matplotlib.pyplot as plt
except ImportError:
    print("Install matplotlib to plot benchmark results: python -m pip install matplotlib")
    raise SystemExit(1)

path = sys.argv[1] if len(sys.argv) > 1 else "results/benchmarks/benchmark.csv"
rows = list(csv.DictReader(open(path, newline="")))
groups = defaultdict(list)
for row in rows:
    key = (row["drivers"], row["riders"])
    groups[key].append((int(row["threads"]), float(row["matching_time_seconds"])))

for key, points in groups.items():
    points.sort()
    plt.figure()
    plt.plot([x for x, _ in points], [y for _, y in points], marker="o")
    plt.xlabel("OpenMP threads")
    plt.ylabel("Matching time (seconds)")
    plt.title(f"Drivers={key[0]}, Riders={key[1]}")
    plt.grid(True)
    plt.savefig(f"results/benchmarks/threads_{key[0]}_{key[1]}.png", dpi=150, bbox_inches="tight")
    plt.close()
