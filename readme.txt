================================================================================
                    DSA String Compression Tool — README
================================================================================

Group Project: String Compression
Course: CSC10004 - Data Structures & Algorithms
Term: Summer 2026

--------------------------------------------------------------------------------
1. COMPILATION
--------------------------------------------------------------------------------

This project requires the GNU g++ compiler with C++17 support.

Method 1 — Direct g++ compilation (RECOMMENDED for grading):

    g++ src/*.cpp -o compressor -std=c++17

Method 2 — CMake build:

    mkdir build && cd build
    cmake ..
    make

Both methods produce a `compressor` executable in the project root.

--------------------------------------------------------------------------------
2. USAGE
--------------------------------------------------------------------------------

    ./compressor -a [algorithm] -m [mode] -i [input_file] -o [output_file]

Options:
    -a [algorithm]   Select algorithm: rle, huff, lzw, lz77
    -m [mode]        Select mode: c (compress), d (decompress)
    -i [input_file]  Path to the source file
    -o [output_file] Path to the resulting file

Supported algorithms:
    rle   — Run-Length Encoding
    huff  — Huffman Coding
    lzw   — Lempel-Ziv-Welch
    lz77  — LZ77 (bonus algorithm)

--------------------------------------------------------------------------------
3. EXAMPLES
--------------------------------------------------------------------------------

Compress a text file using LZW:

    ./compressor -a lzw -m c -i tests/sample_input.txt -o output.lzw

Decompress back to text:

    ./compressor -a lzw -m d -i output.lzw -o restored.txt

Compress with RLE:

    ./compressor -a rle -m c -i tests/sample_input.txt -o output.rle

Compress with Huffman:

    ./compressor -a huff -m c -i tests/sample_input.txt -o output.huff

Compress with LZ77 (bonus):

    ./compressor -a lz77 -m c -i tests/sample_input.txt -o output.lz77

--------------------------------------------------------------------------------
4. PERFORMANCE OUTPUT
--------------------------------------------------------------------------------

After each compression, the tool prints a summary to stdout:

    Compression complete.
    --------------------------------
    Algorithm: LZW
    Execution Time: 0.05 ms
    Original Size: 20 bytes
    Compressed Size: 8 bytes
    Compression Ratio: 2.50
    Space Savings: 60.0%

--------------------------------------------------------------------------------
5. PROJECT STRUCTURE
--------------------------------------------------------------------------------

    DSA-String-Compression/
    ├── CMakeLists.txt
    ├── readme.txt              (this file)
    ├── src/
    │   ├── compressor.hpp      — ICompressor abstract interface
    │   ├── bitio.hpp           — Bit-stream I/O for LZW
    │   ├── main.cpp            — CLI entry point & performance metrics
    │   ├── lzw.hpp / lzw.cpp   — LZW algorithm (Phat)
    │   ├── Huffman.h / .cpp    — Huffman algorithm (Nhat Tien)
    │   ├── rle.h / rle.cpp     — RLE algorithm (Quang Tien)
    │   └── bonus_lz77.h / .cpp — LZ77 bonus algorithm (Quang Tien)
    ├── tests/
    │   ├── sample_input.txt    — Running example: BABBACAC
    │   └── scenario2/          — Entropy-varying test data
    └── data/
        └── (test output files)

--------------------------------------------------------------------------------
6. TEAM MEMBERS
--------------------------------------------------------------------------------

    Phat   — Group Leader & Lead Programmer (CLI, LZW, architecture)
    Khanh  — QA, Experimental Lead & Final Submitter
    Nhat Tien — Huffman Coding Specialist
    Quang Tien — RLE Developer & Bonus LZ77

================================================================================
