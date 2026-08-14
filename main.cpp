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