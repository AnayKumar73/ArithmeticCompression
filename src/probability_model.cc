#include "probability_model.h"

ProbabilityModel::ProbabilityModel(const std::string& data) {
    for (char c : data) {
        frequencies_[c]++;
        if(std::find(symbols_.begin(), symbols_.end(), c) == symbols_.end()) {
            symbols_.push_back(c);
        }
    }
    total_ = 0;
    for (char c : symbols_) {
        cumulative_[c] = total_;
        total_ += frequencies_[c];
    }
}

int ProbabilityModel::getFrequency(char c) const {
        return frequencies_.at(c);
}

int ProbabilityModel::getCumulative(char c) const {
    return cumulative_.at(c);
}

int ProbabilityModel::getTotal() const {
    return total_;
}

std::vector<char> ProbabilityModel::getSymbols() const {
    return symbols_;
}