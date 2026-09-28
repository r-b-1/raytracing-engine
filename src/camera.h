#ifndef CAMERA_H
#define CAMERA_H

#include <algorithm>
#include <limits>
#include <string>

#include "framebuffer.h"
#include "color.h"

#include "ray.h"
#include "shape.h"


class camera{
    public:
    float aspect_ratio = 1;
    int image_width = 256;
    int image_height = 256;

    // R is the record type the world produces -- SphereHitStruct for spheres,
    // TriangleHitStruct for a mesh.
    //
    // A list holds one shape family, so a scene mixing spheres and triangles
    // needs one render() call per list. `filename` keeps those from clobbering
    // each other on disk.
    template<typename R>
    void render(const Shape& world, const std::string& filename = "image.png") {
        initialize();
        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                auto u = double(i);
                auto v = double(j);
                auto pixel_color = ray_color<R>(ray(camera_center,
                                                      pixel00_loc + u*pixel_delta_u
                                                      + v*pixel_delta_v - camera_center),
                                                 world);
                framebuffer.setPixel(i, j, pixel_color);
            }
        }
        framebuffer.exportAsPNG(filename);
    }

    private:
    point3 camera_center = point3(0, 0, 0);
    vec3 pixel_delta_u, pixel_delta_v, pixel00_loc;

    template<typename R>
    color ray_color(const ray& r, const Shape& world) const {
        double tmax = std::numeric_limits<double>::infinity();
        R rec;
        if (world.intersect(r, k_tmin, tmax, rec)) {
            return 0.5 * (rec.normal() + color(1, 1, 1));
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    // Nearest hit the camera will accept. The week 4 slides suggest 1.0, to
    // reject geometry at or in front of the image plane, but this scene's
    // sphere sits at t ~= 0.5, so that would blank the whole image. A small
    // epsilon just stops a ray from registering the surface it starts on.
    static constexpr double k_tmin = 0.001;

    Framebuffer framebuffer;


    void initialize() {
        // Calculate the image height to ensure that its at least 1.
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        framebuffer = Framebuffer(image_width, image_height);

        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    }
};


#endif
