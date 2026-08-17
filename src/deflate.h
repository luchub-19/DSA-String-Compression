#pragma once

#include <string>
#include "compressor.hpp"

bool compressDeflate(const std::string& inputPath, const std::string& outputPath);
bool decompressDeflate(const std::string& inputPath, const std::string& outputPath);

class DeflateCompressor : public ICompressor {
public:
    void compress(const std::string& inputPath, const std::string& outputPath) override;
    void decompress(const std::string& inputPath, const std::string& outputPath) override;
    std::string name() const override { return "Deflate"; }
};
