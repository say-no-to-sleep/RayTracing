#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

int main() {
    // === WORLD ===

    hittable_list world;

    auto R = std::cos(pi/4);

    // materials
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);

    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8));
    auto material_left_fuzzy = make_shared<metal>(color(0.8, 0.8, 0.8), 0.2);            // refcount = 1
    auto material_left_dielectric = make_shared<dielectric>(1.5);
    // Assuming the world is water and this is a bubble
    auto material_left_dielectric_bubble = make_shared<dielectric>(1.00 / 1.33);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2));
    auto material_right_fuzzy = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left_dielectric));   // sphere copies this -> 2
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    // world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right_fuzzy));
    
    world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(make_shared<sphere>(point3( R, 0, -1), R, material_right));

    // === CAMERA ===


    camera cam;
    
    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 960;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 90;

    cam.render(world);
    
}