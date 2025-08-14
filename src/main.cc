#include "arithmetic_encoder.h"
#include "arithmetic_decoder.h"
#include "probability_model.h"
#include <fstream>
#include <iostream>

int main() {
    // Test 1: Basic string
    std::string message1 = "Hello World!$";
    std::cout << "\nEncoding: " << message1 << std::endl;
    
    ProbabilityModel model1(message1);
    std::ofstream output1("test1.bin", std::ios::binary);
    ArithmeticEncoder encoder1(output1);
    encoder1.encode(message1, model1);
    output1.close();
    
    std::ifstream input1("test1.bin", std::ios::binary);
    ArithmeticDecoder decoder1(input1, model1);
    std::cout << "Decoding: ";
    decoder1.decode();
    std::cout << std::endl;
    input1.close();
    
    // Test 2: Repetitive characters
    std::string message2 = "aaaaabbbbbcccc$";
    std::cout << "\nEncoding: " << message2 << std::endl;
    
    ProbabilityModel model2(message2);
    std::ofstream output2("test2.bin", std::ios::binary);
    ArithmeticEncoder encoder2(output2);
    encoder2.encode(message2, model2);
    output2.close();
    
    std::ifstream input2("test2.bin", std::ios::binary);
    ArithmeticDecoder decoder2(input2, model2);
    std::cout << "Decoding: ";
    decoder2.decode();
    std::cout << std::endl;
    input2.close();
    
    // Test 3: Single character
    std::string message3 = "x$";
    std::cout << "\nEncoding: " << message3 << std::endl;
    
    ProbabilityModel model3(message3);
    std::ofstream output3("test3.bin", std::ios::binary);
    ArithmeticEncoder encoder3(output3);
    encoder3.encode(message3, model3);
    output3.close();
    
    std::ifstream input3("test3.bin", std::ios::binary);
    ArithmeticDecoder decoder3(input3, model3);
    std::cout << "Decoding: ";
    decoder3.decode();
    std::cout << '\n' << std::endl;
    input3.close();
    
    return 0;
}
