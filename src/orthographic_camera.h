#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

#include "camera.h"

class OrthographicCamera : public camera {
protected:
    ray make_ray(const point3& pixel_position) const override {
        // Rays start at different grid positions but all travel forward.
        // Parallel rays keep an object's apparent size independent of depth.
        return ray(pixel_position, -w);
    }
};

#endif
