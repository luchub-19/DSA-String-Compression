// Common interface for every compression algorithm (RLE, Huffman, LZW, ...).
// Owner: Phat (CLI framework)

#pragma once

#include <string>

class ICompressor {
public:
    virtual ~ICompressor() = default;

    // Throws std::runtime_error on failure.
    virtual void compress(const std::string& inputPath, const std::string& outputPath) = 0;
    virtual void decompress(const std::string& inputPath, const std::string& outputPath) = 0;

    virtual std::string name() const = 0; // e.g. "LZW"
};