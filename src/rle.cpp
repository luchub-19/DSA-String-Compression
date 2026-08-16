#include "rle.h"
#include <fstream>
#include <vector>
#include <cstdint>
#include <iostream>
using namespace std;

bool compressRLE(const string& inputPath, const string& outputPath) {
    ifstream inFile(inputPath, ios::binary);
    if (!inFile.is_open()) return false;

    vector<uint8_t> buffer((istreambuf_iterator<char>(inFile)),
                                 istreambuf_iterator<char>());
    inFile.close();

    ofstream outFile(outputPath, ios::binary);
    if (!outFile.is_open()) return false;

    if (buffer.empty()) {
        outFile.close();
        return true;
    }

    size_t n = buffer.size();
    size_t i = 0;
    while (i < n) {
        uint8_t currentChar = buffer[i];
        uint8_t count = 1;

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

bool decompressRLE(const string& inputPath, const string& outputPath) {
    ifstream inFile(inputPath, ios::binary);
    if (!inFile.is_open()) return false;

    ofstream outFile(outputPath, ios::binary);
    if (!outFile.is_open()) return false;

    char ch;
    char countChar;
    while (inFile.get(ch) && inFile.get(countChar)) {
        uint8_t count = static_cast<uint8_t>(countChar);
        for (uint8_t i = 0; i < count; ++i) {
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