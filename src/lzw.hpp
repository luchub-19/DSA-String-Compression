// lzw.hpp
//
// Lempel-Ziv-Welch (LZW) compressor/decompressor.
//
// Owner: Phat

#pragma once

#include "compressor.hpp"

class LZWCompressor : public ICompressor {
public:
    void compress(const std::string& inputPath, const std::string& outputPath) override;
    void decompress(const std::string& inputPath, const std::string& outputPath) override;
    std::string name() const override { return "LZW"; }

private:
    // Codes start at 9 bits (256 single-byte entries already occupy 0..255,
    // so the first multi-byte entry is code 256) and grow up to 16 bits.
    // At 16 bits the dictionary holds at most 65536 entries; once full it
    // freezes (stops learning new entries) but keeps compressing with the
    // existing table, which is the classic LZW behaviour used by Unix
    // `compress` and GIF.
    static constexpr int kMinCodeWidth = 9;
    static constexpr int kMaxCodeWidth = 16;
    static constexpr int kDictLimit = 1 << kMaxCodeWidth; // 65536
};
