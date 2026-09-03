#!/usr/bin/env python3
from collections import Counter

W = (("R7", 49), ("R9", 49), ("TT", 49), ("HS", 49), ("JP", 2))
TOTAL = sum(w for _, w in W)


def pick(rng: int) -> str:
    n = rng % TOTAL
    for name, w in W:
        if n < w:
            return name
        n -= w
    return "R7"


def main() -> None:
    trials = 198_000
    state = 1
    counts: Counter[str] = Counter()
    for _ in range(trials):
        state = (1664525 * state + 1013904223) & 0xFFFFFFFF
        counts[pick(state)] += 1
    for name, w in W:
        print(f"{name:3} {counts[name]:7d}  {counts[name]/trials:7.3%}  want {w/TOTAL:7.3%}")
    jp = counts["JP"] / trials
    if not (0.005 <= jp <= 0.02):
        raise SystemExit("jackpot rate off")
    print("ok")


if __name__ == "__main__":
    main()
