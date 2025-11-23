#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

void output(ostream &outStream, const vec3& color) {
    outStream << static_cast<int>(255.999 * clamp(color.x(), 0.0, 1.0)) << " "
       << static_cast<int>(255.999 * clamp(color.y(), 0.0, 1.0)) << " "
       << static_cast<int>(255.999 * clamp(color.z(), 0.0, 1.0)) << "\n";
}

void outputColor(ostream &outStream, color pixelColor, int samplesPerPixel) {
    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    bool gamaCorrected = false;
    color col = (pixelColor / samplesPerPixel);
    output(outStream, gamaCorrected ? col.sqrtv() : col);
}

#endif

