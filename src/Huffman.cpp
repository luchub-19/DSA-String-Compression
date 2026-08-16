#define _CRT_SECURE_NO_WARNINGS
#include "Huffman.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>

struct HuffmanNode {
    unsigned char ch;
    unsigned long long freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char character, unsigned long long frequency)
        : ch(character), freq(frequency), left(nullptr), right(nullptr) {
    }
    ~HuffmanNode() {
        delete left;
        delete right;
    }
};
inline bool isSmaller(HuffmanNode* a, HuffmanNode* b) {
    if (a->freq != b->freq) return a->freq < b->freq;
    return a->ch < b->ch; 
}
class MinHeap {
private:
    std::vector<HuffmanNode*> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (isSmaller(heap[index], heap[parent])) {
                HuffmanNode* temp = heap[index];
                heap[index] = heap[parent];
                heap[parent] = temp;
                index = parent;
            }
            else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int size = (int)heap.size();
        while (2 * index + 1 < size) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int smallest = index;

            if (isSmaller(heap[leftChild], heap[smallest])) {
                smallest = leftChild;
            }
            if (rightChild < size && isSmaller(heap[rightChild], heap[smallest])) {
                smallest = rightChild;
            }
            if (smallest != index) {
                HuffmanNode* temp = heap[index];
                heap[index] = heap[smallest];
                heap[smallest] = temp;
                index = smallest;
            }
            else {
                break;
            }
        }
    }

public:
    int size() const { return (int)heap.size(); }

    void push(HuffmanNode* node) {
        heap.push_back(node);
        heapifyUp((int)heap.size() - 1);
    }
    HuffmanNode* pop() {
        if (heap.empty()) return nullptr;
        HuffmanNode* rootNode = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return rootNode;
    }
};

class BitWriter {
private:
    FILE* outFile;
    unsigned char buffer;
    int bitCount;
public:
    BitWriter(FILE* f) : outFile(f), buffer(0), bitCount(0) {}
    void writeBit(int bit) {
        if (bit) {
            buffer |= (1 << (7 - bitCount));
        }
        bitCount++;
        if (bitCount == 8) {
            fwrite(&buffer, 1, 1, outFile);
            buffer = 0;
            bitCount = 0;
        }
    }

    void writeCode(const std::string& code) {
        for (char c : code) {
            writeBit(c == '1' ? 1 : 0);
        }
    }

    void flush() {
        if (bitCount > 0) {
            fwrite(&buffer, 1, 1, outFile);
            buffer = 0;
            bitCount = 0;
        }
    }
};

class BitReader {
private:
    FILE* inFile;
    unsigned char buffer;
    int bitCount;
public:
    BitReader(FILE* f) : inFile(f), buffer(0), bitCount(8) {}

    int readBit() {
        if (bitCount == 8) {
            if (fread(&buffer, 1, 1, inFile) < 1) return -1;
            bitCount = 0;
        }
        int bit = (buffer >> (7 - bitCount)) & 1;
        bitCount++;
        return bit;
    }
};

void generateCodes(HuffmanNode* root, const std::string& currentCode, std::string codes[256]) {
    if (!root) return;
    if (!root->left && !root->right) {
        codes[root->ch] = currentCode;
        return;
    }
    generateCodes(root->left, currentCode + "0", codes);
    generateCodes(root->right, currentCode + "1", codes);
}

bool compressHuffman(const std::string& inputPath, const std::string& outputPath) {
    FILE* inFile = fopen(inputPath.c_str(), "rb");
    if (!inFile) return false;
    unsigned long long freq[256] = { 0 };
    unsigned long long totalBytes = 0;
    unsigned char ch;

    while (fread(&ch, 1, 1, inFile) == 1) {
        freq[ch]++;
        totalBytes++;
    }

    if (totalBytes == 0) {
        fclose(inFile);
        FILE* outFile = fopen(outputPath.c_str(), "wb");
        if (outFile) fclose(outFile);
        return true;
    }

    MinHeap heap;
    unsigned int uniqueSymbols = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            heap.push(new HuffmanNode((unsigned char)i, freq[i]));
            uniqueSymbols++;
        }
    }

    while (heap.size() > 1) {
        HuffmanNode* left = heap.pop();
        HuffmanNode* right = heap.pop();
        HuffmanNode* parent = new HuffmanNode(0, left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        heap.push(parent);
    }

    HuffmanNode* root = heap.pop();
    
    std::string codes[256];
    if (uniqueSymbols == 1) {
        codes[root->ch] = "0";
    }
    else {
        generateCodes(root, "", codes);
    }

    FILE* outFile = fopen(outputPath.c_str(), "wb");
    if (!outFile) {
        delete root;
        fclose(inFile);
        return false;
    }

    fwrite(&totalBytes, sizeof(totalBytes), 1, outFile);
    unsigned short K = (unsigned short)uniqueSymbols;
    fwrite(&K, sizeof(K), 1, outFile);

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            unsigned char sym = (unsigned char)i;
            fwrite(&sym, 1, 1, outFile);
            fwrite(&freq[i], sizeof(freq[i]), 1, outFile);
        }
    }

    fseek(inFile, 0, SEEK_SET);
    BitWriter writer(outFile);

    while (fread(&ch, 1, 1, inFile) == 1) {
        writer.writeCode(codes[ch]);
    }
    writer.flush();

    delete root;
    fclose(inFile);
    fclose(outFile);
    return true;
}

bool decompressHuffman(const std::string& inputPath, const std::string& outputPath) {
    FILE* inFile = fopen(inputPath.c_str(), "rb");
    if (!inFile) return false;

    unsigned long long totalBytes = 0;
    if (fread(&totalBytes, sizeof(totalBytes), 1, inFile) < 1) {
        fclose(inFile);
        FILE* outFile = fopen(outputPath.c_str(), "wb");
        if (outFile) fclose(outFile);
        return true;
    }

    unsigned short K = 0;
    fread(&K, sizeof(K), 1, inFile);

    unsigned long long freq[256] = { 0 };
    for (int i = 0; i < K; i++) {
        unsigned char sym;
        unsigned long long f;
        fread(&sym, 1, 1, inFile);
        fread(&f, sizeof(f), 1, inFile);
        freq[sym] = f;
    }

    MinHeap heap;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            heap.push(new HuffmanNode((unsigned char)i, freq[i]));
        }
    }

    if (heap.size() == 0) {
        fclose(inFile);
        return true;
    }

    while (heap.size() > 1) {
        HuffmanNode* left = heap.pop();
        HuffmanNode* right = heap.pop();
        HuffmanNode* parent = new HuffmanNode(0, left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        heap.push(parent);
    }

    HuffmanNode* root = heap.pop();

    FILE* outFile = fopen(outputPath.c_str(), "wb");
    if (!outFile) {
        delete root;
        fclose(inFile);
        return false;
    }

    BitReader reader(inFile);
    HuffmanNode* current = root;
    unsigned long long decodedBytes = 0;

    if (K == 1) {
        while (decodedBytes < totalBytes) {
            fwrite(&(root->ch), 1, 1, outFile);
            decodedBytes++;
        }
    }
    else {
        while (decodedBytes < totalBytes) {
            int bit = reader.readBit();
            if (bit == -1) break;

            if (bit == 0) current = current->left;
            else current = current->right;

            if (!current->left && !current->right) {
                fwrite(&(current->ch), 1, 1, outFile);
                decodedBytes++;
                current = root;
            }
        }
    }

    delete root;
    fclose(inFile);
    fclose(outFile);
    return true;
}

// ICompressor adapter
void HuffmanCompressor::compress(const std::string& inputPath, const std::string& outputPath) {
    if (!compressHuffman(inputPath, outputPath)) {
        throw std::runtime_error("Huffman compression failed (check input file).");
    }
}

void HuffmanCompressor::decompress(const std::string& inputPath, const std::string& outputPath) {
    if (!decompressHuffman(inputPath, outputPath)) {
        throw std::runtime_error("Huffman decompression failed (corrupted or invalid input).");
    }
}