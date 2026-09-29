#include <memory>

#include "camera.h"
#include "scene.h"
#include "sphere.h"
#include "triangle.h"
#include "vec3.h"

int main() {
    camera cam;

    Scene scene;

    // Both shape types belong to one scene. The closest hit determines which
    // surface is visible at each pixel, regardless of insertion order.
    scene.objects.push_back(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    scene.objects.push_back(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    scene.objects.push_back(std::make_shared<triangle>(point3(-0.4, -0.6, -1),
                                                       point3(0.4, -0.6, -1),
                                                       point3(0.0, 0.6, -2)));
    scene.objects.push_back(std::make_shared<triangle>(point3(0.7, 0.3, -1),
                                                       point3(0.7, 0.5, -1),
                                                       point3(0.0, 0.4, -3)));

    cam.render(scene, "scene.png");
}
