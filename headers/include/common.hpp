#ifndef COMMON_HPP
#define COMMON_HPP

#include "vec2.hpp"
#include <cstdlib>
#include <limits>
#include <vector>
#include <cstring>
#include <sstream>
#include <string>
#include <memory>

using namespace std;

const double infinity = numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

inline double randomDouble() { // [0,1)
    return rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max) { // min,max).
    return min + (max-min)*randomDouble();
}

inline vector <string> split(string text){
    vector <string> result;
    stringstream ss(text);
    string word;
    while (ss >> word) {
        result.push_back(word);
    }
    return result;
}

// Limita um valor entre um mínimo e máximo
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#endif // !COMMON_HPP