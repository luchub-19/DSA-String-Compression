// lzw.cpp
//
// Owner: Phat
//
// Dynamic dictionary is stored as:
//   - unordered_map<string,int>  (string -> code) while compressing, since
//     we need fast "is this string already known?" lookups.
//   - vector<string>             (code -> string) while decompressing,
//     since codes are dense integers assigned in order, so a vector gives
//     O(1) lookup with no hashing overhead.
// Both grow dynamically as new substrings are learned, exactly like the
// textbook LZW algorithm.

#include "lzw.hpp"
#include "bitio.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

void LZWCompressor::compress(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream in(inputPath, std::ios::binary);
    if (!in) {
        throw std::runtime_error("LZW: cannot open input file: " + inputPath);
    }
    std::ofstream out(outputPath, std::ios::binary);
    if (!out) {
        throw std::runtime_error("LZW: cannot open output file: " + outputPath);
    }

    // Initialize the dictionary with every single-byte string (codes 0..255).
    std::unordered_map<std::string, int> dict;
    dict.reserve(kDictLimit);
    for (int i = 0; i < 256; ++i) {
        dict.emplace(std::string(1, static_cast<char>(i)), i);
    }
    int nextCode = 256;
    int codeWidth = kMinCodeWidth;
    bool dictFull = false;

    LZWBitWriter writer(out);

    std::string w; // longest prefix currently matched in the dictionary
    char byte;
    while (in.get(byte)) {
        std::string wc = w + byte;
        auto it = dict.find(wc);
        if (it != dict.end()) {
            // w + c is already known: keep extending the match.
            w = std::move(wc);
        } else {
            // w is the longest known match: emit its code.
            writer.writeBits(static_cast<std::uint32_t>(dict.at(w)), codeWidth);

            if (!dictFull) {
                dict.emplace(std::move(wc), nextCode);
                ++nextCode;
                // Bump the code width the moment the next code to assign
                // would no longer fit in the current width.
                if (nextCode == (1 << codeWidth) && codeWidth < kMaxCodeWidth) {
                    ++codeWidth;
                }
                if (nextCode >= kDictLimit) {
                    dictFull = true;
                }
            }
            w = std::string(1, byte);
        }
    }
    if (!w.empty()) {
        writer.writeBits(static_cast<std::uint32_t>(dict.at(w)), codeWidth);
    }
    writer.flush();
}

void LZWCompressor::decompress(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream in(inputPath, std::ios::binary);
    if (!in) {
        throw std::runtime_error("LZW: cannot open input file: " + inputPath);
    }
    std::ofstream out(outputPath, std::ios::binary);
    if (!out) {
        throw std::runtime_error("LZW: cannot open output file: " + outputPath);
    }

    // Initialize the dictionary with every single-byte string (codes 0..255).
    std::vector<std::string> dict;
    dict.reserve(kDictLimit);
    for (int i = 0; i < 256; ++i) {
        dict.emplace_back(1, static_cast<char>(i));
    }
    int nextCode = 256;
    int codeWidth = kMinCodeWidth;
    bool dictFull = false;

    LZWBitReader reader(in);

    std::uint32_t code = 0;
    if (!reader.readBits(codeWidth, code)) {
        return; // empty compressed file -> empty output
    }
    if (code >= dict.size()) {
        throw std::runtime_error("LZW: corrupted stream (invalid first code)");
    }
    std::string prev = dict[code];
    out.write(prev.data(), static_cast<std::streamsize>(prev.size()));

    while (reader.readBits(codeWidth, code)) {
        std::string entry;
        if (code < dict.size()) {
            entry = dict[code];
        } else if (code == dict.size()) {
            // Special case: the encoder emitted a code for an entry it had
            // just added to its own dictionary (pattern "wcw...c" repeats).
            entry = prev + prev[0];
        } else {
            throw std::runtime_error("LZW: corrupted stream (invalid code)");
        }

        out.write(entry.data(), static_cast<std::streamsize>(entry.size()));

        if (!dictFull) {
            dict.push_back(prev + entry[0]);
            ++nextCode;
            // NOTE: the decoder is structurally always exactly one insertion
            // "behind" the encoder (it cannot learn the dictionary entry for
            // the code it just read until *after* decoding it), so it must
            // widen its own code width one insertion earlier than the
            // encoder does to stay bit-for-bit aligned on the same stream.
            if (nextCode == (1 << codeWidth) - 1 && codeWidth < kMaxCodeWidth) {
                ++codeWidth;
            }
            if (nextCode >= kDictLimit) {
                dictFull = true;
            }
        }
        prev = std::move(entry);
    }
}
