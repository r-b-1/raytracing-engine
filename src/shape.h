#ifndef SHAPE_H
#define SHAPE_H


#include "ray.h"


class HitRecord {
public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }    
};

class Shape {
public:
    virtual ~Shape() = default;

    virtual bool hit(
        const ray& r,
        double ray_tmin,
        double ray_tmax,
        HitRecord& record
    ) const = 0;
};






#endif