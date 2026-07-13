#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

// Sphere inherits from hittable. (AKA something that can be hit by a ray)
class sphere : public hittable {
    private:
        point3 center;
        double radius;
    public:
        // constructor
        // receives a point and a radius
        // Prevent stored radius of being negative. 
        sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

        // Implements the virtual function from hittable.
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            // oc = C - Q
            vec3 oc = center - r.origin();
            // a = d dot d, dot product of the ray direction with itself
            // gives the squared length of the direction vector
            auto a = dot(r.direction(), r.direction());
            // h = d dot (C - Q)
            auto h = dot(r.direction(), oc);
            // c = (C - Q) dot (C - Q) - r^2
            auto c = dot(oc, oc) - radius * radius;
            // discriminant = h^2 - ac
            auto disc = h * h - a * c;
            // discriminant has to be bigger than zero
            if (disc < 0)
                return false;

            // sqrt(disc)
            auto sqrtd = std::sqrt(disc);
            // find the nearest intersection
            auto root = (h - sqrtd) / a;

            // check if the root is within the range. 
            if (!ray_t.surrounds(root)) {
                // If it is not acceptable, we try the further root
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root))
                    // the root is treated as not hit. 
                    return false;
            }

            // record where the ray hit
            rec.t = root;
            // point is at origin + t * direction
            rec.p = r.at(rec.t);
            // Calculates the outward normal
            vec3 outward_normal = (rec.p - center) / radius;
            // Flip normal if ray is inside->outside.
            rec.set_face_normal(r, outward_normal);


            return true;
        }
};

#endif