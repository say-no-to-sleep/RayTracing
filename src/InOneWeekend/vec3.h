#ifndef VEC3_H
#define VEC3_H


class vec3 {
    public: 
        double e[3];

        // Constructors
        vec3() : e{0,0,0} {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        // Get the coordinates
        double x() const {return e[0];}
        double y() const {return e[1];}
        double z() const {return e[2];}

        // Operator overloading, get negative
        vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
        // Operator overloading, v[i] -> e[i], const type
        double operator[](int i) const {return e[i];}
        // Operator overloading, v[i] -> e[i], non const
        double& operator[](int i) {return e[i];}

        // Overload plus
        vec3& operator +=(const vec3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }
        
        // Overload times
        vec3& operator *=(double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        // Overload divides
        vec3& operator /=(double t) {
            return *this *= 1/t;
        }

        // Get the actual length
        double length() const {
            return std::sqrt(length_squared());
        }

        // Calculates x^2 + y^2 + z^2, useful when comparing lengths or checking vectors have zero length
        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

};

// point3 is alias for vec3, useful for geometric clarity in code.
using point3 = vec3;

// Vector utility functions

// Print?
inline std::ostream& operator <<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Plus 2 numbers together
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Subtract one from the other
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Multiply 2 together
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Multiply a number and a vector together
inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

// The other direction
inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

// Division
inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

// random direction needed for diffuse
inline vec3 random_unit_vector() {
    while (true) {
        // get a random point inside the cube
        auto p = vec3::random(-1,1);
        // calculate how far the point is from the center. 
        auto lensq = p.length_squared();
        // Is the point inside the sphere and not essentially zero
        if (1e-160 < lensq && lensq <= 1) return p / sqrt(lensq); // normalization
    }
}

// set random vector for diffuse to correct hemisphere
inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    // if dot is positive, mean they are on the same side
    if(dot(on_unit_sphere, normal) > 0.0) return on_unit_sphere;
    else return -on_unit_sphere;
}

#endif