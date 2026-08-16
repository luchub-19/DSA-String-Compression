"""
Generates the benchmark datasets required by Section 2.2 of the project
guide.

Scenario 1 (impact of file size): DATA TYPE IS FIXED to standard English
text; only the SIZE varies, from 10 KB up to 10 MB.

Scenario 2 (impact of data entropy): SIZE IS FIXED at ~1 MB; DATA TYPE
varies across three entropy levels: highly repetitive, standard English
prose, and fully randomized characters.
"""

import os
import random
import string

random.seed(42)  # reproducible test data across runs

SCENARIO1_DIR = "tests/scenario1"
SCENARIO2_DIR = "tests/scenario2"

os.makedirs(SCENARIO1_DIR, exist_ok=True)
os.makedirs(SCENARIO2_DIR, exist_ok=True)

KB = 1024
MB = 1024 * 1024

# A small, fixed vocabulary of common English words. Sampling from this
# (with spaces between words) gives text with the kind of skewed,
# word/character-level redundancy real English prose has -- much more
# compressible than fully random text, but far less repetitive than the
# "AAAAABBBBB..." pattern, which is exactly the middle-entropy point
# Scenario 2 needs.
_WORDS = (
    "the quick brown fox jumps over lazy dog cat sat on mat old river bank "
    "big small red blue green tree house car road sun moon star fish bird "
    "run walk swim fly jump sit stand look see hear feel think know go "
    "come here there where when why how what who a an in on at by for with "
    "and but or not so very much more less good bad happy sad fast slow "
    "under over near far left right up down day night morning evening"
).split()


def make_english_text(target_bytes: int) -> str:
    """Word-salad English-like text, sampled i.i.d. from a common-word
    vocabulary, grown until it reaches roughly target_bytes."""
    words = []
    size = 0
    while size < target_bytes:
        w = random.choice(_WORDS)
        words.append(w)
        size += len(w) + 1  # +1 for the separating space
    return " ".join(words)[:target_bytes]


def make_repetitive_text(target_bytes: int) -> str:
    pattern = "AAAAABBBBBCCCCCDDDDD"
    reps = target_bytes // len(pattern) + 1
    return (pattern * reps)[:target_bytes]


def make_random_text(target_bytes: int) -> str:
    alphabet = string.ascii_letters + string.digits
    return "".join(random.choices(alphabet, k=target_bytes))


def write(path: str, content: str) -> None:
    with open(path, "w", encoding="utf-8") as f:
        f.write(content)
    print(f"  wrote {path} ({os.path.getsize(path)} bytes)")


print("Scenario 1: file size sweep (data type fixed = standard English)")
scenario1_sizes = {
    "english_10kb.txt": 10 * KB,
    "english_100kb.txt": 100 * KB,
    "english_1mb.txt": 1 * MB,
    "english_10mb.txt": 10 * MB,
}
for filename, size in scenario1_sizes.items():
    write(os.path.join(SCENARIO1_DIR, filename), make_english_text(size))

print("Scenario 2: data entropy sweep (size fixed = 1 MB)")
scenario2_files = {
    "repetitive_1mb.txt": make_repetitive_text(1 * MB),
    "english_1mb.txt": make_english_text(1 * MB),
    "random_1mb.txt": make_random_text(1 * MB),
}
for filename, content in scenario2_files.items():
    write(os.path.join(SCENARIO2_DIR, filename), content)

print("-> Da khoi tao xong cac bo du lieu test trong thu muc tests/")