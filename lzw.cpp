#include "lzw.h"
#include "utils.h"
#include <sstream>

using namespace std;

int findInDict(const vector<string>& dict, const string& target) {
    for (size_t i = 0; i < dict.size(); i++) {
        if (dict[i] == target) return i;
    }
    return -1;
}

string lzwCompress(const string& input) {
    if (input.empty()) return "";
    vector<string> dict;
    for (int i = 0; i < 256; i++) dict.push_back(string(1, char(i)));

    string p = "";
    ostringstream oss;
    for (size_t i = 0; i < input.length(); i++) {
        char c = input[i];
        string pc = p + c;
        if (findInDict(dict, pc) != -1) {
            p = pc;
        } else {
            oss << findInDict(dict, p) << ",";
            dict.push_back(pc);
            p = string(1, c);
        }
    }
    if (!p.empty()) oss << findInDict(dict, p);
    return oss.str();
}

// Logic giải nén LZW tiêu chuẩn
string lzwDecompress(const string& input) {
    if (input.empty()) return "";
    vector<string> dict;
    for (int i = 0; i < 256; i++) dict.push_back(string(1, char(i)));

    vector<int> codes;
    string temp = "";
    for (size_t i = 0; i < input.length(); i++) {
        if (input[i] == ',') {
            codes.push_back(stringToInt(temp));
            temp = "";
        } else {
            temp += input[i];
        }
    }
    if (!temp.empty()) codes.push_back(stringToInt(temp));

    if (codes.empty()) return "";

    ostringstream oss;
    string prev = dict[codes[0]];
    oss << prev;

    for (size_t i = 1; i < codes.size(); i++) {
        int code = codes[i];
        string curr = "";
        if (code < dict.size()) {
            curr = dict[code];
        } else if (code == dict.size()) {
            curr = prev + prev[0];
        }
        oss << curr;
        dict.push_back(prev + curr[0]);
        prev = curr;
    }
    return oss.str();
}