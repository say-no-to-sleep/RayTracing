#ifndef HITTABLE_H
#define HITTABLE_H

class material;

// stores information about where and how a ray hit something
class hit_record {
    public:
        // point in space where ray hits object
        point3 p;
        // surface normal vector at that point
        vec3 normal;
        // material
        shared_ptr<material> mat;
        // how far along the ray it was
        double t;

        // front face
        bool front_face;

        // calculates front_face and the normal vector
        void set_face_normal(const ray& r, const vec3& outward_normal) {

            // If dot is negative, ray and outward normal points towards each other, ray is hitting outside, front_face is true
            front_face = dot(r.direction(), outward_normal) < 0;
            // If front face is true, it's normal. If it's not (we hit from inside), we flip the normal. 
            normal = front_face ? outward_normal : -outward_normal;
        }
};

//  Virtual functions:
//  member function that can be overridden by derived class, and when you call it through a base class pointer/reference.
//  When we use 
//      virtual bool hit (...) const = 0;
//  when a class that inherit from this and implement, we use
//      virtual bool hit (...) const override { //implementation }

// Define interface that any hittable object must implement
class hittable {
    public:
        // virtual destructor becuase objects may be deleted through hittable* pointer
        // hittable* object = new sphere();
        // delete object;
        virtual ~hittable() = default;
        
        // 
        virtual bool hit(
            const ray& r, // the ray being tested. Passed by reference to avoid copying
            
            interval ray_t, // intersection is valid when ray_tmin <= t, t <= ray_tmax
                                // ray_tmin is set to small positive number. avoid treating surface that emitted a secondary ray as intersecting itself.
                                    // When calculating reflecction, the new ray should begin at the surface
                                    // However due to floating point, it can be slightly inside the surface.
                                    // Therefore the intersection might see it crosses the same sphere immediately again
                                    // Commonly called the Shadow Acne
                                // ray_tmax is distance to the closest intersection found so far. Ignore farther objects
            hit_record& rec // output parameter.
        ) const = 0;
};
#endif