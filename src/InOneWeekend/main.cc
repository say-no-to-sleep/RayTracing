#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

int main() {
    // === WORLD ===

    hittable_list world;
    // sphere in front of us
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    // sphere below us
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // === CAMERA ===

    camera cam;
    
    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 1920;
    cam.samples_per_pixel = 100;

    cam.render(world);
    
}