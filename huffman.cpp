#include "huffman.h"

// Tớ để sẵn khung chạy chuỗi giả lập, thành viên số 2 sẽ ráp Min-Heap vào đây
string huffCompress(const string& input) {
    if (input.empty()) return "";
    // TODO: Áp dụng logic Min-Heap đã bàn trước đó ở đây
    // Trả về chuỗi nén (VD: kèm theo thông tin tần suất ký tự + mã nhị phân)
    return "HUFFMAN_COMPRESSED_MOCK_DATA"; 
}

string huffDecompress(const string& input) {
    if (input.empty()) return "";
    // TODO: Đọc tần suất, dựng lại cây, duyệt mã nhị phân để ra chuỗi gốc
    return "HUFFMAN_DECOMPRESSED_MOCK_DATA";
}