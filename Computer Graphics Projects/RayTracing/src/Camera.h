//
// Created by Alperen Bağ on 19.03.2020.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include "utils.h"

class Camera {

public:
    Camera():u_vec(1,0,0),v_vec(0,1,0),w_vec(0,0,1), d_vec(0,0,1){};
    Vector3D u_vec, v_vec, w_vec, d_vec;

    // i and j are the index of the pixel through which ray goes.
    Ray generate_ray(int i, int j, int res_h, int res_w,  float screen_h, float screen_w, float screen_d);


};


#endif //RAYTRACING_CAMERA_H
