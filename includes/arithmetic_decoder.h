#ifndef ARITHMETIC_DECODER_H
#define ARITHMETIC_DECODER_H

#include <cstdint>
#include <iostream>
#include "probability_model.h"


class ArithmeticDecoder {
public:
    ArithmeticDecoder(std::istream& in, const ProbabilityModel& probabilities);

    void decode();

    char decodeSymbol();
    

private:
    static constexpr uint64_t TOP = 0xFFFFFFFF; //0b111... 
    static constexpr uint64_t HALF = 0x80000000; //equivalent to 0b1000...
    static constexpr uint64_t QUARTER = 0x40000000; //equivalent to 0b01000...

    std::istream& input_;
    const ProbabilityModel& model_;

    uint64_t low_;
    uint64_t high_;
    uint64_t value_;


    uint8_t buffer_;
    int bitsAvailable_;

    int readBit();

    void renormalize();
};

#endif