#include <iostream>
#include "camera.h"

int main() {

  camera cam;


    // Render

    std::cout << "P3\n" << cam.image_width << ' ' << cam.image_height << "\n255\n";

    for (int j = cam.image_height; j >  0; j--) {
        for (int i = 0; i < cam.image_width; i++) {
            auto r = double(i) / (cam.image_width-1);
            auto g = double(j) / (cam.image_height-1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}