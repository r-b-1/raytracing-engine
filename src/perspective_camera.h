#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "camera.h"

class PerspectiveCamera : public camera {
protected:
    ray make_ray(const point3& pixel_position) const override {
        vec3 offset = pixel_position - camera_center;
        // All rays start at the eye and spread across a plane one unit forward.
        // Since w points backward, -w points toward the scene.
        return ray(camera_center, offset - w);
    }
};

#endif
