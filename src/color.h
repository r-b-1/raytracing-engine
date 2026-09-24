#ifndef COLOR_H
#define COLOR_H

using color = vec3;

    auto r = double(i) / (cam.image_width-1);
    auto g = double(j) / (cam.image_height-1);
    auto b = 0.0;

    int ir = int(255.999 * r);
    int ig = int(255.999 * g);
    int ib = int(255.999 * b);


#endif