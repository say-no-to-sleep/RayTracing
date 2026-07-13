#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>

// allow the code to write shared_ptr<hittable> rather than std::shared_ptr<hittable>.
// as of 6.5, we're not using make_shared yet... 
// as of 6.7, we have moved them to rtweekend.h

// inherits from hittable
// Allows code to work with a single abstract object World even if it's a collection of objects
class hittable_list : public hittable {
    public:
        // pointers to hittable objects
        std::vector<shared_ptr<hittable>> objects;

        // default constructor is empty
        hittable_list() {}
        // create a list of 1 object, actual insertion in `add`
        hittable_list(shared_ptr<hittable> object) { add(object); }

        // destroys the vector’s copies of the shared pointers
        void clear() { objects.clear(); }

        // copies the shared pointer
        void add(shared_ptr<hittable> object) {
            // append it to the list of hittable objects
            objects.push_back(object);
        }
        
        // same hit operation for every shape
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            // temporary storage
            hit_record temp_rec;
            // was any intersection found
            bool hit_anything = false;
            // closest_so_far is the maximum distance currently worth considering
            // at the beginning, no intersection has been found, so any hit up to the caller’s original ray_tmax is acceptable
            // when an object was hit, it updates to the closer one and then so on and so forth
            auto closest_so_far = ray_t.max;

            // examine every object.
            for (const auto& object : objects) {
                // ask the OBJECT if the ray hits it within interval from tmin to closest so far (which updates)
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    // it was hit
                    // we hit SOMETHING
                    hit_anything = true;
                    // shrinks the ray distance to this object
                    // future objects must be closer than this to be accounted for
                    closest_so_far = temp_rec.t;
                    // Save the closest hit record found so far.
                    rec = temp_rec;
                }
            }
            // true if at least 1 item was hit
            return hit_anything;
        }
};

#endif