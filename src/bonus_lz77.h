// bonus_lz77.h
//
// LZ77 (bonus) compressor/decompressor.
// Owner: Quang Tien (core algorithm)
// Adapter wrapper: Phat (ICompressor integration)

#pragma once

#include <string>
#include "compressor.hpp"

// --- Original free-function API (Quang Tien's implementation) ---
bool compressLZ77(const std::string& inputPath, const std::string& outputPath);
bool decompressLZ77(const std::string& inputPath, const std::string& outputPath);

// --- ICompressor adapter (wraps the free functions above) ---
class LZ77Compressor : public ICompressor {
public:
    void compress(const std::string& inputPath, const std::string& outputPath) override;
    void decompress(const std::string& inputPath, const std::string& outputPath) override;
    std::string name() const override { return "LZ77"; }
};