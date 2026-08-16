// Huffman.h
//
// Huffman coding compressor/decompressor.
// Owner: Nhat Tien (core algorithm)
// Adapter wrapper: Phat (ICompressor integration)

#pragma once
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include "compressor.hpp"

// --- Original free-function API (Nhat Tien's implementation) ---
bool compressHuffman(const std::string& inputPath, const std::string& outputPath);
bool decompressHuffman(const std::string& inputPath, const std::string& outputPath);

// --- ICompressor adapter (wraps the free functions above) ---
class HuffmanCompressor : public ICompressor {
public:
    void compress(const std::string& inputPath, const std::string& outputPath) override;
    void decompress(const std::string& inputPath, const std::string& outputPath) override;
    std::string name() const override { return "Huffman"; }
};

#endif // HUFFMAN_H