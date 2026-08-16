// Bit-stream writer/reader for packing LZW's variable-width codes (9-16 bits).
// Owner: Phat (LZW)

#pragma once

#include <cstdint>
#include <fstream>

class LZWBitWriter {
public:
    explicit LZWBitWriter(std::ofstream& out) : out_(out) {}

    // Writes low `numBits` bits of value, MSB first.
    void writeBits(std::uint32_t value, int numBits) {
        for (int i = numBits - 1; i >= 0; --i) {
            std::uint8_t bit = static_cast<std::uint8_t>((value >> i) & 1u);
            buffer_ = static_cast<std::uint8_t>((buffer_ << 1) | bit);
            ++bitCount_;
            if (bitCount_ == 8) {
                out_.put(static_cast<char>(buffer_));
                buffer_ = 0;
                bitCount_ = 0;
            }
        }
    }

    // Call once after the last writeBits(); pads remaining bits with 0.
    void flush() {
        if (bitCount_ > 0) {
            buffer_ = static_cast<std::uint8_t>(buffer_ << (8 - bitCount_));
            out_.put(static_cast<char>(buffer_));
            buffer_ = 0;
            bitCount_ = 0;
        }
    }

private:
    std::ofstream& out_;
    std::uint8_t buffer_ = 0;
    int bitCount_ = 0;
};

class LZWBitReader {
public:
    explicit LZWBitReader(std::ifstream& in) : in_(in) {}

    // Reads numBits bits (MSB first) into value; false on end-of-stream.
    bool readBits(int numBits, std::uint32_t& value) {
        value = 0;
        for (int i = 0; i < numBits; ++i) {
            if (bitCount_ == 0) {
                int c = in_.get();
                if (c == std::char_traits<char>::eof()) {
                    return false;
                }
                buffer_ = static_cast<std::uint8_t>(c);
                bitCount_ = 8;
            }
            std::uint8_t bit = static_cast<std::uint8_t>((buffer_ >> 7) & 1u);
            buffer_ = static_cast<std::uint8_t>(buffer_ << 1);
            --bitCount_;
            value = (value << 1) | bit;
        }
        return true;
    }

private:
    std::ifstream& in_;
    std::uint8_t buffer_ = 0;
    int bitCount_ = 0;
};