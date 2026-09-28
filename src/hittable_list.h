#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "shape.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public Shape {
  public:
    std::vector<shared_ptr<Shape>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<Shape> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<Shape> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const override {
        HitRecord temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;

        for (const auto& object : objects) {
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif