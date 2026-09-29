#include <memory>

#include "camera.h"
#include "scene.h"
#include "sphere.h"
#include "triangle.h"
#include "vec3.h"

int main() {
    camera cam;

    Scene scene;

    cam.lookfrom = point3(1, 1, 0);
    cam.lookat = point3(0, 0, -1.5); // Center of your box

    // Both shape types belong to one scene. The closest hit determines which
    // surface is visible at each pixel, regardless of insertion order.
    // scene.objects.push_back(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    // scene.objects.push_back(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    // scene.objects.push_back(std::make_shared<sphere>(point3(0, 0, -1.5), 0.25));


    // // Front Face
    // scene.objects.push_back(std::make_shared<triangle>(point3(0, 0, -1),
    //                                                    point3(-0.5, -0.5, -2), 
    //                                                    point3(-0.5, 0.5, -2)));
    // scene.objects.push_back(std::make_shared<triangle>(point3(0, 0, -1), 
    //                                                    point3(0.5, 0.5, -2), 
    //                                                    point3(0.5, -0.5, -2)));
    // scene.objects.push_back(std::make_shared<triangle>(point3(0, 0, -1),
    //                                                    point3(-0.5, 0.5, -2), 
    //                                                    point3(0.5, 0.5, -2)));
    // scene.objects.push_back(std::make_shared<triangle>(point3(0, 0, -1), 
    //                                                    point3(-0.5, -0.5, -2), 
    //                                                    point3(0.5, -0.5, -2)));


    // Front Face
    scene.objects.push_back(std::make_shared<triangle>(point3(-0.5, -0.5, -1), // Bottom left
                                                       point3(0.5, -0.5, -1), // Bottom Right
                                                       point3(-0.5, 0.5, -1))); // Top Left
    scene.objects.push_back(std::make_shared<triangle>(point3(0.5, 0.5, -1), // Top Right
                                                       point3(-0.5, 0.5, -1), // Top Left
                                                       point3(0.5, -0.5, -1))); // Bottom Right
    // Back Face
    scene.objects.push_back(std::make_shared<triangle>(point3(-0.5, -0.5, -2), // Bottom left
                                                       point3(0.5, -0.5, -2), // Bottom Right
                                                       point3(-0.5, 0.5, -2))); // Top Left
    scene.objects.push_back(std::make_shared<triangle>(point3(0.5, 0.5, -2), // Top Right
                                                       point3(-0.5, 0.5, -2), // Top Left
                                                       point3(0.5, -0.5, -2))); // Bottom Right


    // Bottom Face
    scene.objects.push_back(std::make_shared<triangle>(point3(-0.5, -0.5, -2), // Bottom left
                                                       point3(0.5, -0.5, -2), // Bottom Right
                                                       point3(-0.5, -0.5, -1))); // Top Left
    scene.objects.push_back(std::make_shared<triangle>(point3(0.5, -0.5, -1), // Top Right
                                                       point3(-0.5, -0.5, -1), // Top Left
                                                       point3(0.5, -0.5, -2))); // Bottom Right
    // Top Face
    scene.objects.push_back(std::make_shared<triangle>(point3(-0.5, 0.5, -1), // Bottom left
                                                       point3(0.5, 0.5, -1), // Bottom Right
                                                       point3(-0.5, 0.5, -2))); // Top Left
    scene.objects.push_back(std::make_shared<triangle>(point3(0.5, 0.5, -2), // Top Right
                                                       point3(-0.5, 0.5, -2), // Top Left
                                                       point3(0.5, 0.5, -1))); // Bottom Right
    
    // Right Face
    scene.objects.push_back(std::make_shared<triangle>(point3(0.5, -0.5, -1), // Bottom left
                                                       point3(0.5, -0.5, -2), // Bottom Right
                                                       point3(0.5, 0.5, -1))); // Top Left
    scene.objects.push_back(std::make_shared<triangle>(point3(0.5, 0.5, -2), // Top Right
                                                       point3(0.5, 0.5, -1), // Top Left
                                                       point3(0.5, -0.5, -2))); // Bottom Right    
                                                       
    // Left Face
    scene.objects.push_back(std::make_shared<triangle>(point3(-0.5, -0.5, -1), // Bottom left
                                                       point3(-0.5, -0.5, -2), // Bottom Right
                                                       point3(-0.5, 0.5, -1))); // Top Left
    scene.objects.push_back(std::make_shared<triangle>(point3(-0.5, 0.5, -2), // Top Right
                                                       point3(-0.5, 0.5, -1), // Top Left
                                                       point3(-0.5, -0.5, -2))); // Bottom Right  

    cam.render(scene, "scene.png");
}
