#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    // RGB values for the colour
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate [0,1] to [0,255]
    // We use 255.999 so that when 0.99999... gets multiplied
    // It resolves to 255 instead of 254, this catches issues
    // with floating points.
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out pixel colour components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif