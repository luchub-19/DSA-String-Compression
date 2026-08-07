#include "rle.h"
#include <sstream>

using namespace std;

string rleCompress(const string& input) {
    if (input.empty()) return "";
    ostringstream oss; 
    int count = 1;
    for (size_t i = 1; i < input.length(); ++i) {
        if (input[i] == input[i - 1]) {
            count++;
        } else {
            oss << count << input[i - 1];
            count = 1;
        }
    }
    oss << count << input[input.length() - 1];
    return oss.str();
}

string rleDecompress(const string& input) {
    if (input.empty()) return "";
    ostringstream oss;
    int count = 0;
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] >= '0' && input[i] <= '9') {
            count = count * 10 + (input[i] - '0');
        } else {
            for (int j = 0; j < count; ++j) oss << input[i];
            count = 0; 
        }
    }
    return oss.str();
}