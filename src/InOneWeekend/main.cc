#include <iostream>
#include "color.h"
#include "vec3.h"
#include "ray.h"

// Simple sphere
double hit_sphere(const point3& center, double radius, const ray &r) {
    // Computes C - Q, ray origin to sphere center.
    vec3 oc = center - r.origin();
    // a = d dot d, dot product of the ray direction with itself
    // gives the squared length of the direction vector
    auto a = dot(r.direction(), r.direction());
    // b = -2d dot (C - Q)
    auto b = -2* dot(r.direction(), oc);
    // c = (C - Q) dot (C - Q) - r^2
    auto c = dot(oc, oc) - radius * radius;
    // discriminant = b^2 - 4ac
    auto disc = b * b - 4 * a * c;
    // discriminant has to be bigger than zero

    // 
    if (disc < 0) {
        return -1.0;
    } else {
        // Return the smaller root
        return (-b - std::sqrt(disc)) / (2.0 * a);
    }

    return disc;
}

// color object
color ray_color(const ray& r) {
    // sphere at 0, 0, -1, radius 0.5
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    // Filters out:
    // t = -1: ray missed sphere
    // t < 0:  sphere is behind the camera (should not happen here)
    if (t > 0.0) {
        // r.at(t) computes the position where ray hits the sphere
        // subtract sphere center from that point
        // normalizes it
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        // Normal is [-1,1], colour is [0,1], need to convert it.
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }


    // Get the unit direction
    vec3 unit_direction = unit_vector(r.direction());
    // get the y component of unit vector
    // a = (y+1)/2 (0 <= a <= 1)
    auto a = 0.5*(unit_direction.y() + 1.0);
    // linear interpolation (lerp)between 2 colours
    // white to light blue
    // result = (1-a)*start + a*end
    return (1.0-a)*color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate image height
    // Ensure it is at least 1
    int image_height = int(image_width/aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera settings
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    // We are not using aspect_ratio here because the actual
    // aspect of the image will be different
    auto viewporth_width = viewport_height * (double(image_width)/image_height);
    // We're putting camera in the center for now
    auto camera_center = point3(0,0,0);

    // Calculate vectors across horizontal and down vertical viewport edges
    auto viewport_u = vec3(viewporth_width, 0, 0);
    // the y value is inverted.
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    // viewport sits some distance from the camera along -z axis
    // focal length is that distance
    // -viewport_u/2 -> point left
    // -viewport_v/2 -> points up
    auto viewport_upper_left = camera_center 
                - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    // location of the first pixel
    // move bottom right half the distance of pixel step
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        for(int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height-j) << ' ' << std::flush; 
        for(int i = 0; i < image_width; i++) {
            // Get the center of the current pixel
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            // Get the direction of that specific ray
            auto ray_direction = pixel_center - camera_center;
            // We construct a ray from the camera center towards ray direction
            ray r(camera_center, ray_direction);
            
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                   \n";
}