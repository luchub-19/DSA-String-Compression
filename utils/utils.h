#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream> // Bắt buộc phải có để đọc/ghi file thực tế

using namespace std;

struct RunMetrics {
    double exec_time_ms;
    long long original_size;
    long long compressed_size;
};

string readFile(const string& filepath);
void writeFile(const string& filepath, const string& content);
void printPerformanceSummary(const string& algo, const RunMetrics& metrics);
int stringToInt(const string& s); // Hàm tự viết thay thế std::stoi để tránh lỗi thư viện

#endif