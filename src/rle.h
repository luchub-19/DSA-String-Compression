// rle.h
//
// Run-Length Encoding compressor/decompressor.
// Owner: Quang Tien (core algorithm)
// Adapter wrapper: Phat (ICompressor integration)

#pragma once
#ifndef RLE_H
#define RLE_H

#include <string>
#include "compressor.hpp"

// --- Original free-function API (Quang Tien's implementation) ---
bool compressRLE(const std::string& inputPath, const std::string& outputPath);
bool decompressRLE(const std::string& inputPath, const std::string& outputPath);

// --- ICompressor adapter (wraps the free functions above) ---
class RLECompressor : public ICompressor {
public:
    void compress(const std::string& inputPath, const std::string& outputPath) override;
    void decompress(const std::string& inputPath, const std::string& outputPath) override;
    std::string name() const override { return "RLE"; }
};

#endif // RLE_H