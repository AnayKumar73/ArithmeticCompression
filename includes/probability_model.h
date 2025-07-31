#ifndef PROBABILITY_MODEL_H
#define PROBABILITY_MODEL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>

class ProbabilityModel {
public:

    ProbabilityModel(const std::string& data);

    int getFrequency(char c) const;

    int getCumulative(char c) const;

    int getTotal() const;

    std::vector<char> getSymbols() const;

private:
    std::unordered_map<char, int> frequencies_;
    std::unordered_map<char, int> cumulative_;
    std::vector<char> symbols_;
    int total_;
};

#endif