#ifndef CAMERA_H
#define CAMERA_H

#include <limits>
#include <string>

#include "framebuffer.h"
#include "color.h"

#include "ray.h"
#include "scene.h"


class camera{
    public:

    // Allows a derived camera to be destroyed through a camera pointer.
    virtual ~camera() = default;

    float aspect_ratio = 1;
    int image_width = 500;
    int image_height = 500;
    point3 lookfrom = point3(0, 0, 0);  // Camera position
    point3 lookat = point3(0, 0, -1); // Point to aim toward
    vec3 vup = vec3(0, 1, 0);    // Preferred upward direction

    // One render visits every shape in the scene for each camera ray.
    void render(const Scene& scene, const std::string& filename = "image.png") {
        initialize();
        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                point3 pixel_position =
                    pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
                // The derived camera chooses the ray's origin and direction.
                ray r = make_ray(pixel_position);
                color pixel_color = ray_color(r, scene);
                framebuffer.setPixel(i, j, pixel_color);
            }
        }
        framebuffer.exportAsPNG(filename);
    }


    private:

    color ray_color(const ray& r, const Scene& scene) const {
        double tmax = std::numeric_limits<double>::infinity();
        HitStruct rec;
        if (scene.intersect(r, k_tmin, tmax, rec)) {
            return 0.5 * (rec.normal() + color(1, 1, 1));
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    // A small positive lower bound excludes hits at the ray's own origin.
    static constexpr double k_tmin = 0.001;

    Framebuffer framebuffer;


    void initialize() {
        // Calculate the image height to ensure that its at least 1.
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        framebuffer = Framebuffer(image_width, image_height);

        camera_center = lookfrom;

        // These axes orient the pixel grid. Store w for the derived cameras.
        // lookfrom must differ from lookat; vup must not be parallel to w.
        w = unit_vector(lookfrom - lookat);
        vec3 u = unit_vector(cross(vup, w));
        vec3 v = cross(w, u);

        double viewport_height = 2.0;
        double viewport_width =
            viewport_height * (double(image_width) / image_height);

        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = -viewport_height * v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // This shared grid passes through the camera position. Perspective
        // uses its offsets for directions; orthographic uses it for origins.
        point3 upper_left =
            camera_center - viewport_u / 2 - viewport_v / 2;

        pixel00_loc =
            upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    }

    protected:
    point3 camera_center;
    vec3 w; // Camera's backward direction

    point3 pixel00_loc;
    vec3 pixel_delta_u, pixel_delta_v;

    // This pure virtual function makes camera abstract: each camera type
    // supplies its own projection while sharing the same rendering loop.
    virtual ray make_ray(const point3& pixel_position) const = 0;

};


#endif
