#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include "shape.h"

// A scene owns shapes of any kind through their common Shape interface.
// Later, lights and background settings can live here too.
class Scene {
public:
    std::vector<std::shared_ptr<Shape>> objects;

    // Find the closest hit across ALL shapes for one ray. A miss leaves the
    // caller's record and tmax unchanged; a hit updates both with the winner.
    bool intersect(const ray& r, double tmin, double& tmax, HitStruct& hit) const {
        bool found = false;

        for (const auto& object : objects) {
            HitStruct candidate;
            // Virtual dispatch calls sphere::intersect or triangle::intersect.
            if (object->intersect(r, tmin, tmax, candidate)) {
                tmax = candidate.t();
                hit = candidate;
                found = true;
            }
        }

        return found;
    }
};

#endif
