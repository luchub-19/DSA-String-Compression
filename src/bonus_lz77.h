#pragma once
#include <string>
using namespace std;

bool compressLZ77(const string& inputPath, const string& outputPath);
bool decompressLZ77(const string& inputPath, const string& outputPath);