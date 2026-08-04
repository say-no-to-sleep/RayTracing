#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
    public:
        // aspect rati, width/height
        double aspect_ratio = 1.0;
        // width of image
        int image_width = 100;
        // count of random samples per pixel
        int samples_per_pixel = 10;
        // maximum number of ray bounces
        int max_depth = 10;

        double vfov     = 90;              // Vertical view angle (field of view)
        point3 lookfrom = point3(0,0,0);   // Point camera is looking from
        point3 lookat   = point3(0,0,-1);  // Point camera is looking at
        vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction


        void render(const hittable& world) {
            initialize();
            // === RENDER ===

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
                for(int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height-j) << ' ' << std::flush; 
                for(int i = 0; i < image_width; i++) {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                        // pixel_color += ray_color(r, world);
                    }    
                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }
            std::clog << "\rDone.                   \n";

        }

    private:
        int image_height;
        double pixel_samples_scale;
        point3 center;
        point3 pixel00_loc;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        vec3 u, v, w;

        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;

            // Viewport dimensions
            auto focal_length = (lookfrom - lookat).length();
            auto theta = degree_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2 * h * focal_length;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
            vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge
            
            // Calculate vectors across horizontal and down vertical viewport edges
            // auto viewport_u = vec3(viewport_width, 0, 0);
            // the y value is inverted.
            // auto viewport_v = vec3(0, -viewport_height, 0);

            // Calculate the horizontal and vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            // viewport sits some distance from the camera along -z axis
            // focal length is that distance
            // -viewport_u/2 -> point left
            // -viewport_v/2 -> points up
            auto viewport_upper_left =
                center - (focal_length * w) - viewport_u/2 - viewport_v/2;
            // location of the first pixel
            // move bottom right half the distance of pixel step
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        ray get_ray(int i, int j) const {
            // construct a camera ray originating from the origin and directed at randomly sampled point around the pixel location i, j
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc
                            + ((i + offset.x()) * pixel_delta_u)
                            + ((j + offset.y()) * pixel_delta_v);
            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            // returns vector to a random point in [-.5, -.5,] - [+.5, +.5] unit square
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {
            // if we exceeded ray bounce limit, we do not gather more light
            if (depth <= 0) return color(0,0,0);
            hit_record rec;

            if (world.hit(r, interval(0.001, infinity), rec)){
                // empty variables, to be filled by scatter.
                ray scattered; // new ray direction
                color attenuation; // color

                // ask material, how does this ray bounce.
                // material fills in scattered and attenuation
                // return true if ray bounced. false if absorbed
                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    // multiply this surface's color by whatever color the bounced ray collects 
                    // pixel color = albedo1 x albedo2 x albedo3 x ... x sky color
                    return attenuation * ray_color(scattered, depth-1, world);
                }
                // this is if scatter returned false, return black.
                return color(0,0,0);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }
};

#endif