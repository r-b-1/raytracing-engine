#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>

#include "aabb.h"
#include "shape.h"
#include "vec3.h"

// Record type produced by the sphere family. Kept as its own type so that
// hittable_list and camera have a concrete record to work with even before
// spheres carry anything the base class does not already hold.
class SphereHitStruct : public HitStruct {};

class sphere : public Shape {
public:
    sphere(const point3& center, double radius)
        : center(center), radius(std::fmax(0, radius)) {}

    bool bounding_box(aabb& bounds) const override {
        bounds = aabb(center - vec3(radius, radius, radius),
                      center + vec3(radius, radius, radius));
        return true;
    }

    bool intersect(const ray& r, double tmin, double& tmax, HitStruct& hit) const override {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in [tmin, tmax].
        auto root = (h - sqrtd) / a;
        if (root <= tmin || tmax <= root) {
            root = (h + sqrtd) / a;
            if (root <= tmin || tmax <= root)
                return false;
        }

        hit.set_t(root);
        hit.set_p(r.at(root));
        hit.set_face_normal(r, (r.at(root) - center) / radius);

        return true;
    }

private:
    point3 center;
    double radius;
};

#endif
