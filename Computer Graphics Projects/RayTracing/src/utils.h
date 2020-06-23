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
#include "opencv2/opencv.hpp"


class Vector3D {

public:

    Vector3D(float x, float y, float z);
    float coordinate[3];
    float length;

    float dot(const Vector3D& other);
    Vector3D cross(const Vector3D other);
    Vector3D operator + (const Vector3D other);
    Vector3D operator - (const Vector3D other);
    Vector3D multiply(float scalar);
    Vector3D divide(float scalar);

    void print();
};


class Ray {

public:
    Ray(float x, float y, float z, float d_x, float d_y, float d_z);
    Vector3D origin;
    Vector3D direction_vec;
};


class RGB_pixel {

public:
    RGB_pixel();
    RGB_pixel(float r, float g, float b);
    float value[3];
    RGB_pixel divide(float scalar);
    void print();
};


class RGB_image {

public:
    RGB_image(unsigned int height, unsigned int width); // Initializes an image with black pixels.
    RGB_pixel get_pixel(int i, int j);
    void set_pixel(int i, int j, RGB_pixel value);
    void imwrite(std::string path);
    ~RGB_image();

    int h;
    int w;

private:
    RGB_pixel* data;
};


class Sphere {
public:
    Sphere(float center_x, float center_y, float center_z, float radius, RGB_pixel color);
    Vector3D center;
    float radius;
    RGB_pixel color;
};


class LightSource {
public:
    LightSource(float x, float y, float z):center(x,y,z){};
    Vector3D center;
};

bool hit_check(Ray ray, std::vector<Sphere> spheres, int num_sphere, Vector3D& hit_point, RGB_pixel& hit_color);

#endif //RAYTRACING_UTILS_H
