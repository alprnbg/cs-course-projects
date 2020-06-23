//
// Created by Alperen Bağ on 19.03.2020.
//


#ifndef RAYTRACING_UTILS_H
#define RAYTRACING_UTILS_H

#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <algorithm>
#include "opencv2/opencv.hpp"


class Vector3D {

public:

    Vector3D(float x, float y, float z);
    float coordinate[3];
    float length;

    float dot(const Vector3D& other);
    float cosine_between(const Vector3D& other);
    float distance_between(const Vector3D& other);

    Vector3D cross(const Vector3D other);
    Vector3D operator + (const Vector3D other);
    Vector3D operator - (const Vector3D other);
    Vector3D multiply(float scalar);
    Vector3D divide(float scalar);

};


class Ray {

public:
    Ray(float x, float y, float z, float d_x, float d_y, float d_z);
    Vector3D origin;
    Vector3D direction_vec;
};


class RGB_color {

public:
    RGB_color();
    RGB_color(float r, float g, float b);
    float value[3];
    RGB_color operator + (const RGB_color other);
    RGB_color operator - (const RGB_color other);
    RGB_color divide(float scalar);
};


class RGB_image {

public:
    RGB_image(unsigned int height, unsigned int width); // Initializes an image with black pixels.
    RGB_color get_pixel(int i, int j);
    void set_pixel(int i, int j, RGB_color value);
    void imwrite(std::string path);
    ~RGB_image();

    int h;
    int w;

private:
    RGB_color* data;
};


class Sphere {
public:
    Sphere(float center_x, float center_y, float center_z, float radius, RGB_color color);
    Vector3D center;
    float radius;
    RGB_color color;
};


class LightSource {
public:
    LightSource(float x, float y, float z, float r_i, float b_i, float g_i):center(x,y,z),red_intensity(r_i),green_intensity(g_i),blue_intensity(b_i){};
    Vector3D center;
    float red_intensity;
    float blue_intensity;
    float green_intensity;
};


class Plane {
public:
    Plane(Vector3D unit_normal, Vector3D point, RGB_color color):unit_normal(unit_normal),point(point),color(color){};
    Vector3D unit_normal;
    Vector3D point;
    RGB_color color;
};


class Scene {
public:
    Scene(LightSource light, std::vector<Sphere> spheres, Plane ground);
    LightSource light;
    std::vector<Sphere> spheres;
    Plane ground;
};


bool hit_check(Ray ray, Scene scene, Vector3D& hit_point, Vector3D& hit_normal, RGB_color& hit_color, bool& is_reflective);


RGB_color calculate_diffuse(LightSource light, Vector3D hit_point, Vector3D hit_normal, RGB_color hit_point_color);




#endif //RAYTRACING_UTILS_H
