#include <iostream>

#include "camera.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"

int main() {
    camera cam;

  cam.render();

}

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}