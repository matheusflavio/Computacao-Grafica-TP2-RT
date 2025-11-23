#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

// Escreve um pixel colorido no stream de saída
inline void output(ostream &outStream, const vec3& color) {
    outStream << static_cast<int>(255.999 * clamp(color.x(), 0.0, 1.0)) << " "
       << static_cast<int>(255.999 * clamp(color.y(), 0.0, 1.0)) << " "
       << static_cast<int>(255.999 * clamp(color.z(), 0.0, 1.0)) << "\n";
}

// Escreve a cor de um pixel com anti-aliasing (média de múltiplas amostras)
inline void outputColor(ostream &outStream, color pixelColor, int samplesPerPixel) {
    // Divide a cor pelo número de amostras e aplica correção gamma para gamma=2.0
    bool gamaCorrected = false;
    color col = (pixelColor / samplesPerPixel);
    output(outStream, gamaCorrected ? col.sqrtv() : col);
}

#endif

