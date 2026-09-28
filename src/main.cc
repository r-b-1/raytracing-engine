#include <iostream>

#include "camera.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "shape.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {
  camera cam;

  hittable_list world;

  world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
  // world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    

  cam.render(world);

}
