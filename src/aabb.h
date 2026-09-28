#ifndef AABB_H
#define AABB_H

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include "ray.h"
#include "vec3.h"

// An axis-aligned bounding box. Every Shape must be able to report one; the
// BVH in chapter 7 is built by splitting these, and it also lets a shape be
// rejected wholesale before any per-object intersection test runs.
class aabb {
public:
    point3 min;
    point3 max;

    aabb() : min(0, 0, 0), max(0, 0, 0) {}
    aabb(const point3& min, const point3& max) : min(min), max(max) {}

    point3 origin() const { return min; }
    vec3 direction() const { return max - min; }

    point3 centroid() const { return 0.5 * (min + max); }

    aabb pad(double delta) const {
        return aabb(min - vec3(delta, delta, delta),
                    max + vec3(delta, delta, delta));
    }

    // An inverted box, so that growing from `empty()` with surrounding_box()
    // yields the first real box.
    static aabb empty() {
        double inf = std::numeric_limits<double>::infinity();
        return aabb(point3( inf,  inf,  inf),
                    point3(-inf, -inf, -inf));
    }

    bool is_empty() const {
        return max.x() < min.x() || max.y() < min.y() || max.z() < min.z();
    }

    aabb surrounding_box(const aabb& b) const {
        return aabb(point3(std::fmin(min.x(), b.min.x()),
                           std::fmin(min.y(), b.min.y()),
                           std::fmin(min.z(), b.min.z())),
                    point3(std::fmax(max.x(), b.max.x()),
                           std::fmax(max.y(), b.max.y()),
                           std::fmax(max.z(), b.max.z())));
    }

    // Slab test. Returns true if the ray hits the box somewhere in (t0, t1).
    // Rays with a zero direction component are handled by an explicit
    // parallel test rather than an infinite reciprocal, which would turn
    // 0 * inf into NaN.
    bool intersect(const ray& r, double t0, double t1) const {
        for (int a = 0; a < 3; a++) {
            double origin = r.origin()[a];
            double dir = r.direction()[a];

            if (dir == 0) {
                // Ray is parallel to this slab: it either stays inside it
                // forever or can never enter.
                if (origin < min[a] || origin > max[a]) return false;
                continue;
            }

            double tmin = (min[a] - origin) / dir;
            double tmax = (max[a] - origin) / dir;
            if (dir < 0) std::swap(tmin, tmax);

            t0 = std::fmax(tmin, t0);
            t1 = std::fmin(tmax, t1);
            if (t0 > t1) return false;
        }
        return true;
    }

    static aabb from_cubes(const std::vector<aabb>& cubes) {
        aabb result = aabb::empty();
        for (const auto& b : cubes) {
            if (b.is_empty()) continue;
            result = result.is_empty() ? b : result.surrounding_box(b);
        }
        return result;
    }
};

inline aabb surrounding_box(const aabb& a, const aabb& b) {
    return a.surrounding_box(b);
}

#endif
