#include "probability_model.h"

ProbabilityModel::ProbabilityModel(const std::string& data) {
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