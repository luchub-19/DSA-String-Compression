// #include <iostream>
// #include <fstream>
// #include <cassert>
// #include "rle.h"
// #include "bonus_lz77.h"

// using namespace std;

// void createDummyFile(const string& filename, const string& content) {
//     ofstream out(filename, ios::binary);
//     out << content;
//     out.close();
// }

// bool areFilesEqual(const string& file1, const string& file2) {
//     ifstream f1(file1, ios::binary);
//     ifstream f2(file2, ios::binary);

//     if (!f1.is_open() || !f2.is_open()) return false;

//     char b1, b2;
//     while (f1.get(b1) && f2.get(b2)) {
//         if (b1 != b2) return false;
//     }
//     return f1.eof() == f2.eof();
// }

// int main() {
//     string sampleText = "BABBACACAAAAAAAAAAAAAAABBBBBBBBBBB";
//     createDummyFile("test_input.txt", sampleText);

//     // 1. Test RLE
//     cout << "[1] Testing RLE Algorithm..." << endl;
//     if (compressRLE("test_input.txt", "rle_compressed.bin")) {
//         cout << " -> Compression: SUCCESS" << endl;
//     }
//     if (decompressRLE("rle_compressed.bin", "rle_decompressed.txt")) {
//         cout << " -> Decompression: SUCCESS" << endl;
//     }
//     if (areFilesEqual("test_input.txt", "rle_decompressed.txt")) {
//         cout << " => RLE TEST PASSED: Restored file matches original exactly!" << endl;
//     } else {
//         cout << " => RLE TEST FAILED: Data mismatch!" << endl;
//     }

//     cout << "------------------------------------------" << endl;

//     if (compressLZ77("test_input.txt", "lz77_compressed.bin")) {
//         cout << " -> Compression: SUCCESS" << endl;
//     }
//     if (decompressLZ77("lz77_compressed.bin", "lz77_decompressed.txt")) {
//         cout << " -> Decompression: SUCCESS" << endl;
//     }
//     if (areFilesEqual("test_input.txt", "lz77_decompressed.txt")) {
//         cout << " => LZ77 TEST PASSED: Restored file matches original exactly!" << endl;
//     } else {
//         cout << " => LZ77 TEST FAILED: Data mismatch!" << endl;
//     }
//     return 0;
#include <iostream>
#include <ctime>
#include "Huffman.h"

using namespace std;

// Hàm phụ trợ tính kích thước file (byte) bằng FILE* C-style
long getFileSize(const string& filename) {
    FILE* f = fopen(filename.c_str(), "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);
    return size;
}

int main() {
    string inFile = "test.txt";
    string compressedFile = "test.huff";
    string decompressedFile = "test_restored.txt";

    cout << "=== CHECK TINH DUNG DAN CUA HUFFMAN CODING ===" << endl;

    // 1. CHẠY THỬ NÉN
    clock_t start = clock();
    bool cOk = compressHuffman(inFile, compressedFile);
    clock_t end = clock();

    if (!cOk) {
        cout << "Loi: Khong the nen file! (Kiem tra xem file " << inFile << " co ton tai khong)" << endl;
        return 1;
    }

    double execTime = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    long origSize = getFileSize(inFile);
    long compSize = getFileSize(compressedFile);

    cout << "\n[NEN THANH CONG]" << endl;
    cout << "Thoi gian chay: " << execTime << " ms" << endl;
    cout << "Kich thuoc goc: " << origSize << " bytes" << endl;
    cout << "Kich thuoc nen: " << compSize << " bytes" << endl;
    if (origSize > 0) {
        cout << "Ti le nen (Compression Ratio): " << (double)origSize / compSize << endl;
        cout << "Tiet kiem b/o (Space Savings): " << (1.0 - (double)compSize / origSize) * 100.0 << "%" << endl;
    }

    // 2. CHẠY THỬ GIẢI NÉN
    bool dOk = decompressHuffman(compressedFile, decompressedFile);
    if (dOk) {
        cout << "\n[GIAI NEN THANH CONG]" << endl;
        cout << "File khoi phuc da duoc luu tai: " << decompressedFile << endl;
        cout << "-> Tien hay mo file " << inFile << " va " << decompressedFile << " ra so sanh xem noi dung co giong hệt 100% khong nha!" << endl;
    }
    else {
        cout << "\nLoi: Giai nen THAT BAI!" << endl;
    }

    return 0;
}