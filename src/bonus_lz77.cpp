#include "bonus_lz77.h"
#include <algorithm>
#include <fstream>
#include <vector>
#include <cstdint>
using namespace std;

#pragma pack(push, 1)
struct LZ77Token {
    uint16_t offset;
    uint8_t length;
    uint8_t nextChar;
};
#pragma pack(pop)

bool compressLZ77(const string& inputPath, const string& outputPath) {
    ifstream inFile(inputPath, ios::binary);
    if (!inFile.is_open()) return false;

    vector<uint8_t> data((istreambuf_iterator<char>(inFile)),
                               std::istreambuf_iterator<char>());
    inFile.close();

    ofstream outFile(outputPath, ios::binary);
    if (!outFile.is_open()) return false;

    const uint16_t windowSize = 4095;
    const uint8_t lookaheadSize = 255;

    size_t i = 0;
    size_t n = data.size();

    while (i < n) {
        uint16_t bestMatchOffset = 0;
        uint8_t bestMatchLength = 0;

        size_t searchStart = (i > windowSize) ? (i - windowSize) : 0;

        // Leave >=1 byte unmatched so nextChar is always a real byte, not fabricated.
        size_t remaining = n - i;
        uint8_t maxMatchLen = static_cast<uint8_t>(
            std::min<size_t>(lookaheadSize, remaining - 1));

        for (size_t j = searchStart; j < i; ++j) {
            uint8_t matchLen = 0;
            while (matchLen < maxMatchLen &&
                   data[j + matchLen] == data[i + matchLen]) {
                matchLen++;
            }

            if (matchLen > bestMatchLength) {
                bestMatchLength = matchLen;
                bestMatchOffset = static_cast<uint16_t>(i - j);
            }
        }

        uint8_t nextChar = data[i + bestMatchLength];
        LZ77Token token = {bestMatchOffset, bestMatchLength, nextChar};

        outFile.write(reinterpret_cast<const char*>(&token), sizeof(LZ77Token));
        i += bestMatchLength + 1;
    }

    outFile.close();
    return true;
}

bool decompressLZ77(const string& inputPath, const string& outputPath) {
    ifstream inFile(inputPath, ios::binary);
    if (!inFile.is_open()) return false;

    ofstream outFile(outputPath, ios::binary);
    if (!outFile.is_open()) return false;

    vector<uint8_t> decompressedData;
    LZ77Token token;

    while (inFile.read(reinterpret_cast<char*>(&token), sizeof(LZ77Token))) {
        if (token.length > 0) {
            size_t startPos = decompressedData.size() - token.offset;
            for (uint8_t i = 0; i < token.length; ++i) {
                decompressedData.push_back(decompressedData[startPos + i]);
            }
        }
        decompressedData.push_back(token.nextChar);
    }

    outFile.write(reinterpret_cast<const char*>(decompressedData.data()), decompressedData.size());
    inFile.close();
    outFile.close();
    return true;
}

// ICompressor adapter
#include <stdexcept>

void LZ77Compressor::compress(const std::string& inputPath, const std::string& outputPath) {
    if (!compressLZ77(inputPath, outputPath)) {
        throw std::runtime_error("LZ77 compression failed (check input file).");
    }
}

void LZ77Compressor::decompress(const std::string& inputPath, const std::string& outputPath) {
    if (!decompressLZ77(inputPath, outputPath)) {
        throw std::runtime_error("LZ77 decompression failed (corrupted or invalid input).");
    }
}