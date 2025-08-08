#include <cstdint>
#include <iostream>
#include "probability_model.h"

class ArithmeticEncoder {
public:
    ArithmeticEncoder(std::ostream& out);

    void encode(const std::string& message, const ProbabilityModel& model);

    void encodeSymbol(char c, const ProbabilityModel& model);

    void finish();

private:
    static constexpr uint32_t TOP = 0xFFFFFFFF; //equivalent to 0b1111...
    static constexpr uint32_t HALF = 0x80000000; //equivalent to 0b1000...
    static constexpr uint32_t QUARTER = 0x40000000; //equivalent to 0b01000...

    uint32_t low_, high_;
    int underflowBits_;
    std::ostream& output_;

    uint8_t buffer_;
    int bitsFilled_;

    //Collects bits until enough for a byte, then outputs
    void outputBit(int bit);

    void flushUnderflowBits(int bit);
};