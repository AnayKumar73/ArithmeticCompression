#include "arithmetic_encoder.h"

ArithmeticEncoder::ArithmeticEncoder(std::ostream& out) : low_{0}, high_{TOP}, underflowBits_{0}, output_{out}, buffer_{0}, bitsFilled_{0} { }

void ArithmeticEncoder::encode(const std::string& message, const ProbabilityModel& model) {
    for(char c : message) {
        encodeSymbol(c, model);
    }
    finish();
}

void ArithmeticEncoder::encodeSymbol(char c, const ProbabilityModel& model) {
    int total = model.getTotal();
    uint64_t range = static_cast<uint64_t>(high_) - low_ + 1; //Cast to 64bit to prevent overflow (range is already 32 bits)
    low_ = low_ + static_cast<uint32_t>((range * model.getCumulative(c)) / total);
    high_ = low_ + static_cast<uint32_t>((range * model.getFrequency(c)) / total) - 1;

    while(true) {
        if((low_ & HALF) == (high_ & HALF)) {
            outputBit(low_ >> 31);
            flushUnderflowBits(1 - (low_ >> 31)); //flush opposite bits
        } else if ((low_ & HALF) && !(high_ & HALF)) {
            underflowBits_++;
            low_ &= ~HALF;
            high_ |= QUARTER;
        } else {
            break;
        }
        low_ <<= 1;
        high_ <<= 1;
        high_ |= 1;
    }
}

void ArithmeticEncoder::finish() {
    int bit = low_ >> 31;
    outputBit(bit);
    flushUnderflowBits(1 - bit);

    // Flush rest of low
    for (int i = 1; i < 32; ++i) {
        outputBit((low_ >> (31 - i)) & 1);
    }
    //If some leftover, just fill with zeroes
    if (bitsFilled_ > 0) {
        buffer_ <<= (8 - bitsFilled_);
        output_.put(static_cast<char>(buffer_));
    }
}

//Collects bits until enough for a byte, then outputs
void ArithmeticEncoder::outputBit(int bit) {
    buffer_ <<= 1;
    buffer_ = buffer_ | (bit & 1);
    bitsFilled_++;
    if (bitsFilled_ == 8) {
        output_.put(static_cast<char>(buffer_));
        bitsFilled_ = 0;
        buffer_ = 0;
    }
}

void ArithmeticEncoder::flushUnderflowBits(int bit) {
    while(underflowBits_ > 0) {
        outputBit(bit);
        underflowBits_--;
    }
}