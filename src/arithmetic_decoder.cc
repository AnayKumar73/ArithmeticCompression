#include "arithmetic_decoder.h"

ArithmeticDecoder::ArithmeticDecoder(std::istream& in, const ProbabilityModel& probabilities) :  input_{in}, model_{probabilities}, low_{0}, high_{TOP}, value_{0}, buffer_{0}, bitsAvailable_{0} {
    for (int i = 0; i < 32; ++i) {
        value_ = (value_ << 1) | readBit();
    }
}

void ArithmeticDecoder::decode() {
    while (true) {
        char symbol = decodeSymbol();
        if (symbol == '$')  { //end of file symbol
            break;
        }
        std::cout << symbol;
    }
}

char ArithmeticDecoder::decodeSymbol() {
    std::vector<char> symbols = model_.getSymbols();
    int total = model_.getTotal();

    uint64_t range = high_ - low_ + 1;
    uint64_t scaled = ((value_ - low_) * total - 1) / range;

    for(char c : symbols) {
        uint64_t cumulative = model_.getCumulative(c);
        uint64_t freq = model_.getFrequency(c);

        if(cumulative <= scaled && scaled < cumulative + freq) {
            low_ = low_ + (range * cumulative) / total;
            high_ = low_ + (range * (cumulative + freq)) / total - 1;

            //if most significant bits are equal OR underflow condition
            renormalize(); 
            return c;
        } 
    }
    throw std::runtime_error("Failed to decode symbol");
}

int ArithmeticDecoder::readBit() {
    if (bitsAvailable_ == 0) {
        buffer_ = input_.get();
        bitsAvailable_ = 8;
    }
    //retrieve most significant bit
    int bit = (buffer_ >> 7) & 1; 
    buffer_ <<= 1;
    bitsAvailable_--;
    return bit;
}

void ArithmeticDecoder::renormalize() {
    while((high_ & HALF) == (low_ & HALF) || ((low_ & QUARTER) && !(high_ & QUARTER))) {
        //Most significant bits match
        value_ = ((value_ << 1) & TOP) | readBit();
        low_ = (low_ << 1) & TOP;
        high_ = ((high_ << 1) | 1) & TOP;

        if(((low_ & QUARTER) && !(high_ & QUARTER))) { 
            //underflow E3 case, flips second most significant bit
            value_ ^= QUARTER;
            low_ ^= QUARTER;
            high_ ^= QUARTER;
        }
    }
}