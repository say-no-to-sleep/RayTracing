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
            return false;
        }
}; 


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

class metal : public material {
    public: 
        metal(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            // Instead of lambertian scatter, we have perfect mirror reflection
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            
            // same as lambertian:
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return true;
        }
    private:
        color albedo;
};

#endif