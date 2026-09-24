#include <iostream>
#include "camera.h"
#include "color.h"

int main() {

  camera cam;


    // Render

    std::cout << "P3\n" << cam.image_width << ' ' << cam.image_height << "\n255\n";

    for (int j = cam.image_height; j >  0; j--) {
        std::clog << "\rScanlines remaining: " << (cam.image_height - j) << ' ' << std::flush;
        for (int i = 0; i < cam.image_width; i++) {
            auto pixel_color = color(double(i)/(cam.image_width-1), double(j)/(cam.image_height-1), 0);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";

}