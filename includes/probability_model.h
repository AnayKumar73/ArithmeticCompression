#ifndef PROBABILITY_MODEL_H
#define PROBABILITY_MODEL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>

class ProbabilityModel {
public:

    ProbabilityModel(const std::string& data) {
        for (char c : data) {
            frequencies_[c]++;
            symbols_.push_back(c);
        }

        total_ = 0;
        for (const auto& [c ,count] : frequencies_) {
            cumulative_[c] = total_;
            total_ += count;
        }
        std::sort(symbols_.begin(), symbols_.end(), [&](char a, char b) {
            return cumulative_[a] < cumulative_[b];
        });
    }

    int getFrequency(char c) const {
        return frequencies_.at(c);
    }

    int getCumulative(char c) const {
       return cumulative_.at(c);
    }

    int getTotal() const {
        return total_;
    }

    std::vector<char> getSymbols() const {
        return symbols_;
    }

private:
    std::unordered_map<char, int> frequencies_;
    std::unordered_map<char, int> cumulative_;
    std::vector<char> symbols_;
    int total_;
};

#endif