#include <memory>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "vec3.h"
#include "ray.h"
#include "shape.h"

int main() {
    camera cam;

    // --- sphere scene ---
    hittable_list<SphereHitStruct, sphere> spheres;

    spheres.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    spheres.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    cam.render<SphereHitStruct>(spheres, "sphere.png");

    // --- triangle scene ---
    // The viewport plane is at z = -1, so the triangles have to sit there.
    hittable_list<TriangleHitStruct, triangle> triangles;

    triangles.add(make_shared<triangle>(point3(-0.7, -0.6, -1),
                                        point3( 0.7, -0.6, -1),
                                        point3(-0.7,  0.6, -1)));
    // triangles.add(make_shared<triangle>(point3( 0.7, -0.6, -1),
    //                                     point3( 0.7,  0.6, -1),
    //                                     point3(-0.7,  0.6, -1)));

    cam.render<TriangleHitStruct>(triangles, "triangle.png");

}
