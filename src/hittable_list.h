#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "aabb.h"
#include "shape.h"

using std::make_shared;
using std::shared_ptr;

// R is the concrete record type its members produce -- SphereHitStruct for a
// list of spheres, TriangleHitStruct for a mesh. Templating on R is what keeps
// `hit = temp` from slicing: temp is built as an R, so the whole record is
// copied. H is the shape type, defaulting to Shape.
//
// A list holds one shape family, so all of its members agree on R. Scenes that
// mix families (a ground plane and a mesh) compose two lists.
template<typename R, typename H = Shape>
class hittable_list : public Shape {
public:
    std::vector<shared_ptr<H>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<H> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<H> object) {
        objects.push_back(object);
    }

    bool bounding_box(aabb& bounds) const override {
        if (objects.empty()) {
            bounds = aabb::empty();
            return false;
        }

        if (!objects[0]->bounding_box(bounds)) return false;
        for (size_t i = 1; i < objects.size(); ++i) {
            aabb object_bounds;
            if (!objects[i]->bounding_box(object_bounds)) return false;
            bounds = surrounding_box(bounds, object_bounds);
        }
        return true;
    }

    // `tmax` arrives holding the caller's current closest-so-far distance and
    // is tightened to the closest hit found, so that shapes scanned later in
    // the list can reject anything behind it.
    bool intersect(const ray& r, double tmin, double& tmax, HitStruct& hit) const override {
        R& out = static_cast<R&>(hit);

        bool hit_anything = false;

        for (const auto& object : objects) {
            R temp;
            if (object->intersect(r, tmin, tmax, temp)) {
                hit_anything = true;
                tmax = temp.t();
                out = temp;
            }
        }

        return hit_anything;
    }
};

#endif
