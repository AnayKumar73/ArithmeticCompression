#include "arithmetic_decoder.h"

ArithmeticDecoder::ArithmeticDecoder(std::istream& in, const ProbabilityModel& probabilities) :  input_{in}, model_{probabilities}, low_{0}, high_{TOP}, buffer_{0}, bitsAvailable_{0} {
    value_ = 0;
    for (int i = 0; i < 32; ++i) {`
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

    uint64_t entries = 0;
    entries = high_ - low_ + 1;
    uint64_t target = (value_ - low_) * total;
    //Not dividing target by range because of precision errors, instead just multiply both sides below

    for(char c : symbols) {
        uint64_t cumulative = model_.getCumulative(c);
        uint64_t freq = model_.getFrequency(c);

        if(entries * cumulative <= target && target < entries * (cumulative + freq)) {
            uint64_t new_low = low_ + (entries * cumulative) / total;
            uint64_t new_high = low_ + (entries * (cumulative + freq)) / total - 1;
            low_ = new_low;
            high_ = new_high;

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
    while(true) {
        if((high_ & HALF) == (low_ & HALF)) {
            //msb match
            value_ = ((value_ << 1) & TOP) | readBit();
            low_ = (low_ << 1) & TOP;
            high_ = ((high_ << 1) | 1) & TOP;
        }
        else if((low_ & QUARTER) && !(high_ & QUARTER)) {   
            //underflow E3 condition, flips second msb
            value_ = ((value_ << 1) & TOP) | readBit();
            value_ ^= QUARTER;
            low_ = (low_ << 1) & TOP;
            low_ ^= QUARTER;
            high_ = ((high_ << 1) | 1) & TOP;
            high_ ^= QUARTER;

            value_ &= TOP;
            low_ &= TOP;
            high_ &= TOP;
        }
        else {
            break;
        }
    }
}