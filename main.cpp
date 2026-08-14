#include <iostream>
#include <fstream>
#include <cassert>
#include "rle.h"
#include "bonus_lz77.h"

using namespace std;

void createDummyFile(const string& filename, const string& content) {
    ofstream out(filename, ios::binary);
    out << content;
    out.close();
}

bool areFilesEqual(const string& file1, const string& file2) {
    ifstream f1(file1, ios::binary);
    ifstream f2(file2, ios::binary);

    if (!f1.is_open() || !f2.is_open()) return false;

    char b1, b2;
    while (f1.get(b1) && f2.get(b2)) {
        if (b1 != b2) return false;
    }
    return f1.eof() == f2.eof();
}

int main() {
    string sampleText = "BABBACACAAAAAAAAAAAAAAABBBBBBBBBBB";
    createDummyFile("test_input.txt", sampleText);

    // 1. Test RLE
    cout << "[1] Testing RLE Algorithm..." << endl;
    if (compressRLE("test_input.txt", "rle_compressed.bin")) {
        cout << " -> Compression: SUCCESS" << endl;
    }
    if (decompressRLE("rle_compressed.bin", "rle_decompressed.txt")) {
        cout << " -> Decompression: SUCCESS" << endl;
    }
    if (areFilesEqual("test_input.txt", "rle_decompressed.txt")) {
        cout << " => RLE TEST PASSED: Restored file matches original exactly!" << endl;
    } else {
        cout << " => RLE TEST FAILED: Data mismatch!" << endl;
    }

    cout << "------------------------------------------" << endl;

    if (compressLZ77("test_input.txt", "lz77_compressed.bin")) {
        cout << " -> Compression: SUCCESS" << endl;
    }
    if (decompressLZ77("lz77_compressed.bin", "lz77_decompressed.txt")) {
        cout << " -> Decompression: SUCCESS" << endl;
    }
    if (areFilesEqual("test_input.txt", "lz77_decompressed.txt")) {
        cout << " => LZ77 TEST PASSED: Restored file matches original exactly!" << endl;
    } else {
        cout << " => LZ77 TEST FAILED: Data mismatch!" << endl;
    }
    return 0;
}