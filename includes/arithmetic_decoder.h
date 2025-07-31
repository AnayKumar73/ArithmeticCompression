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
    const uint32_t TOP = 0xFFFFFFFF; //0b111... 
    const uint32_t HALF = 0x80000000; //equivalent to 0b1000...
    const uint32_t QUARTER = 0x40000000; //equivalent to 0b01000...

    std::istream& input_;
    const ProbabilityModel& model_;

    uint32_t low_;
    uint32_t high_;
    uint32_t value_;


    uint8_t buffer_;
    int bitsAvailable_;

    int readBit();

    void renormalize();
};

#endif