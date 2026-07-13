#ifndef INTERVAL_H
#define INTERVAL_H

// interval class
class interval {
    public:
        // minimum and maximum
        double min, max;

        // Default interval is from +infty to -infty, is empty
        interval() : min(+infinity), max(-infinity) {} 

        // Constructor
        interval(double min, double max) : min(min), max(max) {}

        // size of interval
        double size() const {
            return max - min;
        }

        // check if it contains x
        bool contains(double x) const {
            return min <= x && x <= max;
        }

        // check if it surrounds x
        bool surrounds(double x) const {
            return min < x && x < max;
        }

        // clamps a number
        double clamp(double x) const {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        static const interval empty, universe;
};

// empty, nothing is bigger than infinity and smaller than -infinity
const interval interval::empty    = interval(+infinity, -infinity);
// everything is bigger than -infinity and smaller than infinity
const interval interval::universe = interval(-infinity, +infinity);

#endif