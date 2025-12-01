"""Utility script for generating 2,000 sample log lines for large-scale tests."""
from __future__ import annotations

import datetime as _dt
import random
from pathlib import Path

TOTAL_LINES = 2_000
OUTPUT_PATH = Path(__file__).with_name("large.txt")
START_TIME = _dt.datetime(2025, 1, 1, 0, 0, 0)
END_TIME = _dt.datetime(2025, 1, 31, 23, 59, 59)
MODULES = ["Database", "Network", "Auth", "Cache", "Storage", "Scheduler"]
LEVELS = ["INFO", "WARN", "ERROR"]
LEVEL_WEIGHTS = [0.55, 0.30, 0.15]
WORDS = [
    "connection",
"timeout",
"retry",
"success",
    "failure",
    "packet",
    "query",
    "lock",
    "unlock",
    "user",
    "admin",
    "token",
    "cache",
    "commit",
    "rollback",
]


def _random_message() -> str:
    base = f"{random.choice(WORDS)} {random.choice(WORDS)} event detected"
    detail = "".join(random.choices("abcdefghijklmnopqrstuvwxyz0123456789", k=random.randint(10, 25)))
    return f"{base} detail:{detail}"


def main() -> None:
    total_seconds = int((END_TIME - START_TIME).total_seconds())
    step = max(1, total_seconds // TOTAL_LINES)
    current = START_TIME

    with OUTPUT_PATH.open("w", encoding="utf-8") as handle:
        for _ in range(TOTAL_LINES):
            timestamp = current.strftime("%Y-%m-%d %H:%M:%S")
            level = random.choices(LEVELS, weights=LEVEL_WEIGHTS, k=1)[0]
            module = random.choice(MODULES)
            message = _random_message()
            handle.write(f"[{timestamp}] {level} {module} {message}\n")
            current += _dt.timedelta(seconds=step)

    print(f"Wrote {TOTAL_LINES} lines to {OUTPUT_PATH}")


if __name__ == "__main__":
    main()
