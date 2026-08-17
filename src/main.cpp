// CLI entry point: compressor -a [algo] -m [mode] -i [input] -o [output]
// Owner: Phat (CLI framework + performance metrics)

#include "compressor.hpp"
#include "Huffman.h"
#include "lzw.hpp"
#include "rle.h"
#include "bonus_lz77.h"
#include "deflate.h"

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

namespace {

void printUsage(const char* progName) {
    std::cerr << "Usage: " << progName << " -a [algorithm] -m [mode] -i [input_file] -o [output_file]\n\n"
              << "Options:\n"
              << "  -a [algorithm]   Select algorithm: rle, huff, lzw, lz77, deflate\n"
              << "  -m [mode]        Select mode: c (compress), d (decompress)\n"
              << "  -i [input_file]  Path to the source file\n"
              << "  -o [output_file] Path to the resulting file\n";
}

std::unique_ptr<ICompressor> createCompressor(const std::string& algo) {
    if (algo == "lzw")  return std::make_unique<LZWCompressor>();
    if (algo == "rle")  return std::make_unique<RLECompressor>();
    if (algo == "huff") return std::make_unique<HuffmanCompressor>();
    if (algo == "lz77") return std::make_unique<LZ77Compressor>();
    if (algo == "deflate") return std::make_unique<DeflateCompressor>();
    return nullptr;
}

// Parses "-x value" pairs into a map; false on malformed argument.
bool parseArgs(int argc, char** argv, std::unordered_map<std::string, std::string>& out) {
    for (int i = 1; i < argc; ++i) {
        std::string token = argv[i];
        if (token.size() == 2 && token[0] == '-' && i + 1 < argc) {
            out[token] = argv[++i];
        } else {
            std::cerr << "Error: unrecognized or incomplete argument '" << token << "'\n";
            return false;
        }
    }
    return true;
}

void printPerformanceSummary(const std::string& mode,
                              const std::string& algoDisplayName,
                              double elapsedMs,
                              std::uint64_t inputSize,
                              std::uint64_t outputSize) {
    std::cout << (mode == "c" ? "Compression complete." : "Decompression complete.") << '\n';
    std::cout << "--------------------------------\n";
    std::cout << "Algorithm: " << algoDisplayName << '\n';
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Execution Time: " << elapsedMs << " ms\n";

    if (mode == "c") {
        std::cout << "Original Size: " << inputSize << " bytes\n";
        std::cout << "Compressed Size: " << outputSize << " bytes\n";

        double ratio = outputSize > 0
                           ? static_cast<double>(inputSize) / static_cast<double>(outputSize)
                           : 0.0;
        double savingsPct = inputSize > 0
                                 ? (1.0 - static_cast<double>(outputSize) / static_cast<double>(inputSize)) * 100.0
                                 : 0.0;

        std::cout << std::setprecision(2) << "Compression Ratio: " << ratio << '\n';
        std::cout << std::setprecision(1) << "Space Savings: " << savingsPct << "%\n";
    } else {
        std::cout << "Compressed Size: " << inputSize << " bytes\n";
        std::cout << "Decompressed Size: " << outputSize << " bytes\n";
    }
}

} // namespace

int main(int argc, char** argv) {
    std::unordered_map<std::string, std::string> args;
    if (!parseArgs(argc, argv, args)) {
        printUsage(argv[0]);
        return 1;
    }

    if (!args.count("-a") || !args.count("-m") || !args.count("-i") || !args.count("-o")) {
        std::cerr << "Error: missing required arguments (-a, -m, -i, -o are all required).\n";
        printUsage(argv[0]);
        return 1;
    }

    const std::string algo = args["-a"];
    const std::string mode = args["-m"];
    const std::string inputPath = args["-i"];
    const std::string outputPath = args["-o"];

    if (mode != "c" && mode != "d") {
        std::cerr << "Error: mode must be 'c' (compress) or 'd' (decompress), got '" << mode << "'.\n";
        return 1;
    }

    std::unique_ptr<ICompressor> compressor = createCompressor(algo);
    if (!compressor) {
        std::cerr << "Error: unknown algorithm '" << algo << "'. Choose from: rle, huff, lzw, lz77, deflate\n";
        return 1;
    }

    if (!fs::exists(inputPath)) {
        std::cerr << "Error: input file does not exist: " << inputPath << '\n';
        return 1;
    }

    try {
        const auto start = std::chrono::high_resolution_clock::now();
        if (mode == "c") {
            compressor->compress(inputPath, outputPath);
        } else {
            compressor->decompress(inputPath, outputPath);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const double elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();

        const auto inputSize = static_cast<std::uint64_t>(fs::file_size(inputPath));
        const auto outputSize = static_cast<std::uint64_t>(fs::file_size(outputPath));

        printPerformanceSummary(mode, compressor->name(), elapsedMs, inputSize, outputSize);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}