#include "arithmetic_decoder.h"

ArithmeticDecoder::ArithmeticDecoder(std::istream& in, const ProbabilityModel& probabilities) :  input_{in}, model_{probabilities}, low_{0}, high_{TOP}, buffer_{0}, bitsAvailable_{0} {
    value_ = 0;
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
    uint64_t scaled = ((value_ - low_ + 1) * static_cast<uint64_t>(total) - 1) / range;

    for(char c : symbols) {
        uint64_t cumulative = model_.getCumulative(c);
        uint64_t freq = model_.getFrequency(c);

        // std::cerr << "scaled=" << scaled << " cum=" << cumulative << " freq=" << freq << " c=" << c << "\n";
        if (scaled < cumulative + freq) {
            uint64_t new_low = low_ + (range * cumulative) / total;
            uint64_t new_high = low_ + (range * (cumulative + freq)) / total - 1;

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
        int ch = input_.get();
        if (ch == EOF) {
            buffer_ = 0; // pad with zeros at EOF
        } else {
            buffer_ = static_cast<uint8_t>(ch);
        }
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
        else if ((low_ & ~high_ & QUARTER) != 0) {   
            // underflow E3 condition: map middle half to whole by toggling 2nd MSB, then shift
            value_ ^= QUARTER;
            low_ ^= QUARTER;
            high_ ^= QUARTER;

            value_ = ((value_ << 1) & TOP) | readBit();
            low_ = (low_ << 1) & TOP;
            high_ = ((high_ << 1) | 1) & TOP;
        }
        else {
            break;
        }
    }
}