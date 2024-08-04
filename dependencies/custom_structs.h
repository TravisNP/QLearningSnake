#ifndef CUSTOM_STRUCTS_H
#define CUSTOM_STRUCTS_H

#include<iostream>
#include<vector>
#include <boost/unordered/unordered_flat_map.hpp>

/** This class was created in help with chat-gpt and the various sources on the internet
 * I needed a way to use both string and string_view types in my reflection map as
 * I didn't want to create a new string every time I wanted to look at a substring
 */

// Custom hash function for std::string and std::string_view
struct StringViewHash {
    using is_transparent = void; // Enable heterogeneous lookup

    std::size_t operator()(std::string_view sv) const noexcept {
        return std::hash<std::string_view>{}(sv);
    }

    std::size_t operator()(const std::string& s) const noexcept {
        return std::hash<std::string>{}(s);
    }
};

// Custom equality function for std::string and std::string_view
struct StringViewEqual {
    using is_transparent = void; // Enable heterogeneous lookup

    bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
        return lhs == rhs;
    }

    bool operator()(const std::string& lhs, const std::string& rhs) const noexcept {
        return lhs == rhs;
    }

    bool operator()(std::string_view lhs, const std::string& rhs) const noexcept {
        return lhs == rhs;
    }

    bool operator()(const std::string& lhs, std::string_view rhs) const noexcept {
        return lhs == rhs;
    }
};

struct Model {
    std::vector<std::vector<boost::unordered_flat_map<std::string, std::vector<double>>>> qtable;
    int boardSize;
    int numEpisodes;
    int maxRounds;
    int maxEpsilon;
    int minEpsilon;
    int decayRate;
    int gamma;
    int learningRate;

    // Default constructor
    Model()
        : boardSize(0), numEpisodes(0), maxRounds(0), maxEpsilon(0),
          minEpsilon(0), decayRate(0), gamma(0), learningRate(0) {}

    // Parameterized constructor
    Model(std::vector<std::vector<boost::unordered_flat_map<std::string, std::vector<double>>>> _qtable, int _boardSize, int _numEpisodes, int _maxRounds, int _maxEpsilon, int _minEpsilon, int _decayRate, int _gamma, int _learningRate)
        : qtable(_qtable), boardSize(_boardSize), numEpisodes(_numEpisodes), maxRounds(_maxRounds), maxEpsilon(_maxEpsilon),
          minEpsilon(_minEpsilon), decayRate(_decayRate), gamma(_gamma), learningRate(_learningRate) {}

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & qtable;
        ar & boardSize;
        ar & maxRounds;
        ar & maxEpsilon;
        ar & minEpsilon;
        ar & decayRate;
        ar & gamma;
        ar & learningRate;
    }
};

#endif // CUSTOM_STRUCTS_H