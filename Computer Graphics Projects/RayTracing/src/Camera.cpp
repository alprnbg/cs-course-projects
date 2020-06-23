//
// Created by Alperen Bağ on 19.03.2020.
//

#include "Camera.h"



Ray Camera::generate_ray(int i, int j, int res_h, int res_w,  float screen_h, float screen_w, float screen_d) {

    float right = screen_w/2;
    float left = -right;
    float top = screen_h/2;
    float bottom = -top;

    float u = left + (right-left)*(i+0.5)/(float)res_w;
    float v = bottom + (top-bottom)*(j+0.5)/(float)res_h;

    return Ray(0, 0, 0, u, v, screen_d);
}
