#ifndef LZW_H
#define LZW_H
#include <string>
#include <vector>
using namespace std;

string lzwCompress(const string& input);
string lzwDecompress(const string& input);
#endif