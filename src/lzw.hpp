// Lempel-Ziv-Welch (LZW) compressor/decompressor.
// Owner: Phat
#pragma once

#include "compressor.hpp"

class LZWCompressor : public ICompressor {
public:
    void compress(const std::string& inputPath, const std::string& outputPath) override;
    void decompress(const std::string& inputPath, const std::string& outputPath) override;
    std::string name() const override { return "LZW"; }

private:
    // Codes grow 9->16 bits; dictionary freezes once full (classic LZW).
    static constexpr int kMinCodeWidth = 9;
    static constexpr int kMaxCodeWidth = 16;
    static constexpr int kDictLimit = 1 << kMaxCodeWidth;
};