#ifndef SHAPE_H
#define SHAPE_H

#include "aabb.h"
#include "ray.h"

// Base class for everything a ray can intersect. Common data lives here so that
// a shape only needs to downcast to its own record for shape-specific extras
// (triangle barycentrics, material, and so on). A shape stores the *outward*
// normal and flips it in normal(), so the caller never has to care which side
// it came from.
class HitStruct {
public:
    virtual ~HitStruct() = default;

    virtual point3 p() const { return p_; }
    virtual vec3  normal() const { return front_face_ ? outward_normal_ : -outward_normal_; }
    virtual double t() const { return t_; }
    virtual bool  front_face() const { return front_face_; }

    void set_p(const point3& p) { p_ = p; }
    void set_t(double t) { t_ = t; }

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // NOTE: `outward_normal` is assumed to have unit length.
        front_face_ = dot(r.direction(), outward_normal) < 0;
        outward_normal_ = outward_normal;
    }

protected:
    point3 p_ = point3(0, 0, 0);
    double t_ = 0;
    bool   front_face_ = true;
    vec3   outward_normal_ = vec3(0, 0, 0);
};

class Shape {
public:
    virtual ~Shape() = default;

    // Conservative bounds used to reject a shape before intersecting it.
    // Returns false if this shape has no bounds (e.g. an empty list), leaving
    // `bounds` unspecified.
    virtual bool bounding_box(aabb& bounds) const = 0;

    // Fills `hit` with the nearest intersection in [tmin, tmax] and returns
    // true, or returns false and leaves `hit` untouched.
    //
    // `tmax` is an in/out parameter. The caller seeds it with its current
    // closest-so-far distance so that distant shapes are rejected, and it is
    // the caller's job to tighten it after a hit (see hittable_list). Keeping
    // the running bound in `tmax` is what lets a scene be scanned in one pass.
    //
    // Contract: `hit` must be the concrete record type belonging to this
    // shape's family. hittable_list and camera both take that type as a
    // template parameter, so they can honour it.
    virtual bool intersect(
        const ray& r,
        double tmin,
        double& tmax,
        HitStruct& hit
    ) const = 0;
};

#endif
