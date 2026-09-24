#ifndef CAMERA_H
#define CAMERA_H

#include <algorithm>

#include "framebuffer.h"
#include "color.h"

class camera{
    public:
    float aspect_ratio = 1;
    int image_width = 256;
    int image_height = 256;

    void render() {
        initialize();

        // std::cout << "P3\n" << cam.image_width << ' ' << cam.image_height << "\n255\n";

        for (int j = image_height - 1; j >= 0; j--) {
            std::clog << "\rScanlines remaining: " << (j + 1) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                auto pixel_color = color(double(i)/std::max(image_width-1, 1), double(j)/std::max(image_height-1, 1), 0);
                framebuffer.setPixel(i, image_height - 1 - j, pixel_color);
            }
        }
        framebuffer.exportAsPNG("image.png");
        std::clog << "\rDone.                 \n";
    }
    





    private:
    Framebuffer framebuffer;


    void initialize() {
        // Calculate the image height to ensure that its at least 1.
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        framebuffer = Framebuffer(image_width, image_height);

        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width)/image_height);
        auto camera_center = point3(0, 0, 0);


        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        auto pixel_delta_u = viewport_u / image_width;
        auto pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    }
};


#endif
