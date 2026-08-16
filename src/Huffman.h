#pragma once
#ifndef HUFFMAN_H
#define HUFFMAN_H

// Your header file content here
#include <string>

bool compressHuffman(const std::string& inputPath, const std::string& outputPath);
bool decompressHuffman(const std::string& inputPath, const std::string& outputPath);

#endif // HUFFMAN_H