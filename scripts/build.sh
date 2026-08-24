#!/usr/bin/env bash
set -euo pipefail
make clean
make -j"$(nproc 2>/dev/null || echo 2)"
