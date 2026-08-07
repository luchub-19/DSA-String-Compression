#include "utils.h"
#include "rle.h"
#include "lzw.h"
#include "huffman.h"
#include <iostream>
#include <cstring>
#include <chrono> // Bắt buộc dùng để lấy thời gian chạy (ms)

using namespace std;

int main(int argc, char* argv[]) {
    string algo = "", mode = "", input_file = "", output_file = "";
    
    // Đọc tham số dòng lệnh: -a [algo] -m [mode] -i [input] -o [output]
    for (int i = 1; i < argc; i += 2) {
        if (i + 1 >= argc) break;
        if (strcmp(argv[i], "-a") == 0) algo = argv[i+1];
        else if (strcmp(argv[i], "-m") == 0) mode = argv[i+1];
        else if (strcmp(argv[i], "-i") == 0) input_file = argv[i+1];
        else if (strcmp(argv[i], "-o") == 0) output_file = argv[i+1];
    }

    if (algo.empty() || mode.empty() || input_file.empty() || output_file.empty()) {
        cout << "Usage: compressor -a [algorithm] -m [mode] -i [input_file] -o [output_file]\n";
        return 1;
    }

  string input_data = readFile(input_file);
    if (input_data.empty()) {
        cout << "Error: File dữ liệu trống hoặc bạn chưa lưu file!\n";
        return 1; // Thoát nếu không đọc được file
    }

    string output_data = "";
    
    // Bắt đầu đếm thời gian
    auto start = chrono::high_resolution_clock::now();

    // Rẽ nhánh gọi đúng hàm của thuật toán
    if (algo == "rle") {
        if (mode == "c") output_data = rleCompress(input_data);
        else if (mode == "d") output_data = rleDecompress(input_data);
    } 
    else if (algo == "huff") {
        if (mode == "c") output_data = huffCompress(input_data);
        else if (mode == "d") output_data = huffDecompress(input_data);
    } 
    else if (algo == "lzw") {
        if (mode == "c") output_data = lzwCompress(input_data);
        else if (mode == "d") output_data = lzwDecompress(input_data);
    } else {
        cout << "Error: Unknown algorithm. Use rle, huff, or lzw.\n";
        return 1;
    }

    // Kết thúc đếm thời gian
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration = end - start;

    writeFile(output_file, output_data);

    // Tính toán và in thống kê hiệu suất
    RunMetrics metrics;
    metrics.exec_time_ms = duration.count();
    metrics.original_size = input_data.length();
    metrics.compressed_size = output_data.length();
    
    // Chỉ in bảng thống kê khi ở chế độ nén (mode c)
    if (mode == "c") {
        printPerformanceSummary(algo, metrics);
    } else {
        cout << "Decompression complete. Output written to: " << output_file << "\n";
    }

    return 0;
}
