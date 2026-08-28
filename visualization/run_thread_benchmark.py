import csv
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "config" / "config.json"
RESULTS = ROOT / "results" / "thread_benchmark.csv"
EXE = ROOT / "build" / "ride_sim.exe"

THREADS = [1, 2, 4, 6, 8, 10, 12]

original_config = CONFIG.read_text(encoding="utf-8")

rows = []

try:
    for thread_count in THREADS:

        config_text = re.sub(
            r'"matching_mode"\s*:\s*"[^"]+"',
            '"matching_mode": "parallel"',
            original_config
        )

        config_text = re.sub(
            r'"threads"\s*:\s*\d+',
            f'"threads": {thread_count}',
            config_text
        )

        CONFIG.write_text(config_text, encoding="utf-8")

        print(f"\nRunning with {thread_count} threads...")

        result = subprocess.run(
            [str(EXE)],
            cwd=ROOT,
            capture_output=True,
            text=True
        )

        if result.returncode != 0:
            print(result.stdout)
            print(result.stderr)
            raise RuntimeError(
                f"Simulator failed for {thread_count} threads"
            )

        output = result.stdout

        matched = re.search(
            r"Matched rides\s*:\s*(\d+)",
            output
        )

        total_time = re.search(
            r"Total matching time\s*:\s*([\d.]+)",
            output
        )

        average_time = re.search(
            r"Average match/tick\s*:\s*([\d.]+)",
            output
        )

        if not matched or not total_time or not average_time:
            print(output)
            raise RuntimeError(
                f"Could not parse benchmark output for {thread_count} threads"
            )

        row = {
            "threads": thread_count,
            "total_matching_time_ms": float(total_time.group(1)),
            "average_matching_time_ms": float(average_time.group(1)),
            "matched_rides": int(matched.group(1))
        }

        rows.append(row)

        print(
            f"  matched rides : {row['matched_rides']}\n"
            f"  total time    : {row['total_matching_time_ms']} ms\n"
            f"  average/tick  : {row['average_matching_time_ms']} ms"
        )

finally:
    CONFIG.write_text(original_config, encoding="utf-8")


RESULTS.parent.mkdir(parents=True, exist_ok=True)

with RESULTS.open(
    "w",
    newline="",
    encoding="utf-8"
) as file:

    writer = csv.DictWriter(
        file,
        fieldnames=[
            "threads",
            "total_matching_time_ms",
            "average_matching_time_ms",
            "matched_rides"
        ]
    )

    writer.writeheader()
    writer.writerows(rows)

print(f"\nBenchmark saved to: {RESULTS}")
