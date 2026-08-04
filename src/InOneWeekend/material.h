#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(
            const ray& r_in,            // the ray that hit the surface
            const hit_record& rec,      // info about where it hit
            color& attenuation,         // output: how much color the surface absorbs
            ray& scattered              // output: new bounced ray
        ) const {
            return false;               // Default material is an absorber
        }
}; 

// Lambertian material
class lambertian : public material {
    public:
        // albedo: colour of the surface
        lambertian(const color& albedo) : albedo(albedo) {

        }

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            // Lambertian scatter function
            auto scatter_direction = rec.normal + random_unit_vector();

            // catch degenerate scatter direction
            if (scatter_direction.near_zero()) scatter_direction = rec.normal;
 
            // Create the new bounced ray
            scattered = ray(rec.p, scatter_direction);
            // tell ray_color() to multiply color by this surface's albedo.
            attenuation = albedo;
            // lambertian surface always scatter.
            return true;
        }
    
    private:
        color albedo;
};


// Metal material
class metal : public material {
    public: 
        // metal(const color& albedo) : albedo(albedo) {}
        // Add fuzz
        metal(const color& albedo, double fuzz = 0):
            albedo(albedo),
            fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            // Instead of lambertian scatter, we have perfect mirror reflection
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            // Add some kind of fuzz, a unit sphere around the endpoint
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector()); 
            // same as lambertian:
            scattered = ray(rec.p, reflected);
            attenuation = albedo;

            // return true;
            // return if the direction is still outwards instead of going into the sphere
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    private:
        color albedo;
        // Added fuzziness
        double fuzz;
};

// Dielectric material (A.K.A, glass, water, etc)
class dielectric : public material {
    public:
        // material builder
        dielectric(double refraction_index) : refraction_index(refraction_index) {}

        // scatter function, same as everything before.
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            // 1 in every channel -> nothing is absorbed.
            // This is for clear glass. A tinted one would have slight changes to drain colours out
            attenuation = color(1.0, 1.0, 1.0);
            // This ratio inverts depending on which way we cross boundary
            // Because refract takes η/η'
            double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;
                // Entering: front_face = true, ratio is 1.0/1.5
                // Exiting: front_face = false, ratio is just the index = 1.5/1.0

            // This only works because air is assumed to be 1.0

            // We need the unit vector because refracts derived every step assuming it.
            vec3 unit_direction = unit_vector(r_in.direction());

            // Calculates cos theta, same as in refract function.
            double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
            // sin theta = sqrt(1 - cos theta squared), from trig identities.
            double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

            // If (η/η' sin theta) is above 1, the equation has no solution, we fall back to reflect
            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract)
                // reflect
                direction = reflect(unit_direction, rec.normal);
            else
                // refract
                direction = refract(unit_direction, rec.normal, ri);

            // New ray start at hit point and goes to refracted position
            scattered = ray(rec.p, direction);
            // return unconditionally. Dielectric never swallows ray.
            return true;
        }

    private:
        // Refractive index in vacuum or air, or the ratio of the material's refractive index over
        // the refractive index of the enclosing media
        double refraction_index;
};

#endif