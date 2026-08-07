#include "utils.h"

using namespace std;

string readFile(const string& filepath) {
    ifstream file(filepath, ios::binary);
    if (!file) {
        cout << "Error: Cannot open input file " << filepath << "\n";
        return "";
    }
    ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void writeFile(const string& filepath, const string& content) {
    ofstream file(filepath, ios::binary);
    if (file) {
        file << content;
    }
}

// In kết quả theo đúng format yêu cầu của đề
void printPerformanceSummary(const string& algo, const RunMetrics& metrics) {
    cout << "Compression complete.\n";
    cout << "--------------------------------\n";
    cout << "Algorithm: " << algo << "\n";
    cout << "Execution Time: " << metrics.exec_time_ms << " ms\n";
    cout << "Original Size: " << metrics.original_size << " bytes\n";
    cout << "Compressed Size: " << metrics.compressed_size << " bytes\n";
    
    double ratio = 0.0;
    double savings = 0.0;
    
    if (metrics.compressed_size > 0) {
        ratio = (double)metrics.original_size / metrics.compressed_size;
    }
    if (metrics.original_size > 0) {
        savings = (1.0 - (double)metrics.compressed_size / metrics.original_size) * 100.0;
    }
    
    cout << "Compression Ratio: " << ratio << "\n";
    cout << "Space Savings: " << savings << "%\n";
}

int stringToInt(const string& s) {
    int res = 0;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            res = res * 10 + (s[i] - '0');
        }
    }
    return res;
}