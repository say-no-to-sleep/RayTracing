#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <memory>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degree_to_radians(double degree) {
    return degree * pi / 180.0;
}

// returns a random real number in [0,1)
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

// Returns random real in [min, max)
inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif