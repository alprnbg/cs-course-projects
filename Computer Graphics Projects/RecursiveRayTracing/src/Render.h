//
// Created by Alperen Bağ on 10.04.2020.
//

#include "Camera.h"

#ifndef RECURSIVERAYTRACING_RENDER_H
#define RECURSIVERAYTRACING_RENDER_H

void cast_ray(Ray ray, Scene scene, int MAX_DEPTH, RGB_color& color, float& depth);


#endif //RECURSIVERAYTRACING_RENDER_H
