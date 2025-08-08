#include "arithmetic_encoder.h"
#include "arithmetic_decoder.h"
#include "probability_model.h"
#include <fstream>
#include <iostream>

int main() {
    std::string message = "abcdef$";  // Make sure to include EOF symbol, e.g. '$'

    // Build model from the message (for simplicity)
    ProbabilityModel model(message);

    // Open output file in binary mode
    std::ofstream output("secret.bin", std::ios::binary);

    ArithmeticEncoder encoder(output);
    encoder.encode(message, model);
    // output.flush();
    output.close();

    // Open input file in binary mode
    std::ifstream input("secret.bin", std::ios::binary);

    // Use same model to decode
    ArithmeticDecoder decoder(input, model);
    decoder.decode();

    input.close();
    return 0;
}
