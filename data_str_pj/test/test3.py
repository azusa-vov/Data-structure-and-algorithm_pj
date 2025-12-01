import random
import datetime
from pathlib import Path

"""Utility script for generating the large test file described in case 3."""

# General configuration
FILENAME = "test3.txt"
TOTAL_LINES = 50_000
TARGET_SIZE_MB = 5  # informative only, not enforced
START_TIME = datetime.datetime(2025, 1, 1, 0, 0, 0)
END_TIME = datetime.datetime(2025, 1, 31, 23, 59, 59)

# Log field populations
MODULES = ["Database", "Network", "Auth", "Cache", "Storage"]
LEVELS = ["INFO", "WARN", "ERROR"]
LEVEL_WEIGHTS = [0.60, 0.25, 0.15]
KEYWORDS = [
    "connection", "timeout", "failed", "success", "retry", "data",
    "query", "unauthorized", "packet", "lost", "found", "null pointer",
    "overflow", "index", "user", "admin", "login", "logout"
]


def generate_random_message() -> str:
    """Compose a pseudo-random message that keeps each line near the target size."""

    base_msg = (
        f"{random.choice(KEYWORDS)} {random.choice(KEYWORDS)} "
        f"happened during {random.choice(KEYWORDS)} operation"
    )
    padding_length = random.randint(30, 80)
    padding = "".join(
        random.choices("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", k=padding_length)
    )
    return f"{base_msg} details:{padding}"


def main() -> None:
    print(f"Generating {FILENAME} with {TOTAL_LINES} lines...")

    total_seconds = (END_TIME - START_TIME).total_seconds()
    step = total_seconds / TOTAL_LINES
    current_time = START_TIME

    with open(FILENAME, "w", encoding="utf-8") as handle:
        for _ in range(TOTAL_LINES):
            time_str = current_time.strftime("%Y-%m-%d %H:%M:%S")
            level = random.choices(LEVELS, weights=LEVEL_WEIGHTS, k=1)[0]
            module = random.choice(MODULES)
            message = generate_random_message()
            handle.write(f"[{time_str}] {level} {module} {message}\n")
            current_time += datetime.timedelta(seconds=step)

    size_mb = Path(FILENAME).stat().st_size / (1024 * 1024)
    print(f"Done! File {FILENAME} created (approx {size_mb:.2f} MB).")


if __name__ == "__main__":
    main()