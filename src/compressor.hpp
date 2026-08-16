// compressor.hpp
//
// Common interface implemented by every compression algorithm (RLE, Huffman,
// LZW, ...). The CLI framework in main.cpp talks only to this interface, so
// each teammate can implement their own algorithm class independently and
// plug it into the tool without touching main.cpp.
//
// Owner: Phat (CLI framework)

#pragma once

#include <string>

class ICompressor {
public:
    virtual ~ICompressor() = default;

    // Reads the file at inputPath, compresses it, and writes the result to
    // outputPath. Must throw std::runtime_error (or a subclass) on failure
    // (e.g. cannot open file, invalid data).
    virtual void compress(const std::string& inputPath, const std::string& outputPath) = 0;

    // Reads a previously-compressed file at inputPath, decompresses it, and
    // writes the reconstructed original to outputPath. Must throw
    // std::runtime_error on failure (e.g. corrupted/invalid input).
    virtual void decompress(const std::string& inputPath, const std::string& outputPath) = 0;

    // Human readable name used in the performance summary, e.g. "LZW".
    virtual std::string name() const = 0;
};
