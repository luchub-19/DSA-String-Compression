#include "rle.h"
#include <fstream>
#include <vector>
#include <cstdint>
#include <iostream>

bool compressRLE(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream inFile(inputPath, std::ios::binary);
    if (!inFile.is_open()) return false;

    std::vector<std::uint8_t> buffer((std::istreambuf_iterator<char>(inFile)),
                                      std::istreambuf_iterator<char>());
    inFile.close();

    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) return false;

    if (buffer.empty()) {
        outFile.close();
        return true;
    }

    std::size_t n = buffer.size();
    std::size_t i = 0;
    while (i < n) {
        std::uint8_t currentChar = buffer[i];
        std::uint8_t count = 1;

        while (i + 1 < n && buffer[i + 1] == currentChar && count < 255) {
            count++;
            i++;
        }
        
        outFile.put(static_cast<char>(currentChar));
        outFile.put(static_cast<char>(count));
        i++;
    }

    outFile.close();
    return true;
}

bool decompressRLE(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream inFile(inputPath, std::ios::binary);
    if (!inFile.is_open()) return false;

    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) return false;

    char ch;
    char countChar;
    while (inFile.get(ch) && inFile.get(countChar)) {
        std::uint8_t count = static_cast<std::uint8_t>(countChar);
        for (std::uint8_t i = 0; i < count; ++i) {
            outFile.put(ch);
        }
    }

    inFile.close();
    outFile.close();
    return true;
}

// ICompressor adapter
#include <stdexcept>

void RLECompressor::compress(const std::string& inputPath, const std::string& outputPath) {
    if (!compressRLE(inputPath, outputPath)) {
        throw std::runtime_error("RLE compression failed (check input file).");
    }
}

void RLECompressor::decompress(const std::string& inputPath, const std::string& outputPath) {
    if (!decompressRLE(inputPath, outputPath)) {
        throw std::runtime_error("RLE decompression failed (corrupted or invalid input).");
    }
}