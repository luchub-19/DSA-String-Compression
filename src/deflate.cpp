#include "deflate.h"
#include "bonus_lz77.h"
#include "Huffman.h"

#include <cstdio>
#include <filesystem>
#include <random>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

namespace {

std::string makeTempPath(const std::string& nearPath) {
    // Same directory as the real output so the temp file stays on one
    // filesystem (safe to rename/remove), plus a random suffix so two
    // concurrent runs never collide on the same temp name.
    static std::random_device rd;
    std::ostringstream oss;
    oss << nearPath << ".stage" << rd() << ".tmp";
    return oss.str();
}

// Deletes the temp file on scope exit -- success, early return, or
// exception -- so a failed stage never leaves stray .tmp files behind.
class TempFileGuard {
public:
    explicit TempFileGuard(std::string p) : path_(std::move(p)) {}
    ~TempFileGuard() {
        std::error_code ec;
        fs::remove(path_, ec); // best-effort cleanup; ignore failure
    }
    const std::string& path() const { return path_; }

private:
    std::string path_;
};

} // namespace

bool compressDeflate(const std::string& inputPath, const std::string& outputPath) {
    TempFileGuard temp(makeTempPath(outputPath));

    if (!compressLZ77(inputPath, temp.path())) {
        return false;
    }
    if (!compressHuffman(temp.path(), outputPath)) {
        return false;
    }
    return true;
}

bool decompressDeflate(const std::string& inputPath, const std::string& outputPath) {
    TempFileGuard temp(makeTempPath(outputPath));

    if (!decompressHuffman(inputPath, temp.path())) {
        return false;
    }
    if (!decompressLZ77(temp.path(), outputPath)) {
        return false;
    }
    return true;
}

// ICompressor adapter
void DeflateCompressor::compress(const std::string& inputPath, const std::string& outputPath) {
    if (!compressDeflate(inputPath, outputPath)) {
        throw std::runtime_error("Deflate compression failed (check input file).");
    }
}

void DeflateCompressor::decompress(const std::string& inputPath, const std::string& outputPath) {
    if (!decompressDeflate(inputPath, outputPath)) {
        throw std::runtime_error("Deflate decompression failed (corrupted or invalid input).");
    }
}
