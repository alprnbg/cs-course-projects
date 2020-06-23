//
// Created by Alperen Bağ on 19.03.2020.
//

#include "utils.h"


Vector3D::Vector3D(float x, float y, float z) {
    this->coordinate[0] = x;
    this->coordinate[1] = y;
    this->coordinate[2] = z;
    this->length = sqrt(x*x + y*y + z*z);
}


float Vector3D::dot(const Vector3D &other) {
    return this->coordinate[0] * other.coordinate[0]
           + this->coordinate[1] * other.coordinate[1]
           + this->coordinate[2] * other.coordinate[2];
}


float Vector3D::cosine_between(const Vector3D& other){
	return this->dot(other)/(this->length*other.length);
}


float Vector3D::distance_between(const Vector3D& other){
	return sqrt( 
		((this->coordinate[0] - other.coordinate[0])*(this->coordinate[0] - other.coordinate[0])) +
		((this->coordinate[1] - other.coordinate[1])*(this->coordinate[1] - other.coordinate[1])) +
		((this->coordinate[2] - other.coordinate[2])*(this->coordinate[2] - other.coordinate[2])));
}


Vector3D Vector3D::cross(const Vector3D other) {

    float c_x = this->coordinate[1]*other.coordinate[2] - this->coordinate[2]*other.coordinate[1];
    float c_y = this->coordinate[2]*other.coordinate[0] - this->coordinate[0]*other.coordinate[2];
    float c_z = this->coordinate[0]*other.coordinate[1] - this->coordinate[1]*other.coordinate[0];

    return Vector3D(c_x, c_y, c_z);
}


Vector3D Vector3D::operator+(const Vector3D other) {
    return Vector3D(this->coordinate[0] + other.coordinate[0],
                    this->coordinate[1] + other.coordinate[1],
                    this->coordinate[2] + other.coordinate[2]);
}


Vector3D Vector3D::operator-(const Vector3D other) {
    return Vector3D(this->coordinate[0] - other.coordinate[0],
                    this->coordinate[1] - other.coordinate[1],
                    this->coordinate[2] - other.coordinate[2]);
}


Vector3D Vector3D::multiply(float scalar) {
    return Vector3D(this->coordinate[0]*scalar, this->coordinate[1]*scalar, this->coordinate[2]*scalar);
}


Vector3D Vector3D::divide(float scalar) {
    return Vector3D(this->coordinate[0]/scalar, this->coordinate[1]/scalar, this->coordinate[2]/scalar);
}


RGB_color::RGB_color(float r, float g, float b) {
    this->value[0] = r;
    this->value[1] = g;
    this->value[2] = b;
}


RGB_color::RGB_color() {
    this->value[0] = 0;
    this->value[1] = 0;
    this->value[2] = 0;
}



RGB_color RGB_color::divide(float scalar) {
    return RGB_color(this->value[0]/scalar, this->value[1]/scalar, this->value[2]/scalar);
}

RGB_color RGB_color::operator+(const RGB_color other) {
    return RGB_color(this->value[0]+other.value[0], this->value[1]+other.value[1], this->value[2]+other.value[2]);
}

RGB_color RGB_color::operator-(const RGB_color other) {
    return RGB_color(this->value[0]-other.value[0], this->value[1]-other.value[1], this->value[2]-other.value[2]);
}


RGB_image::RGB_image(unsigned int height, unsigned int width) {
    this->h = height;
    this->w = width;
    this->data = new RGB_color[height * width];
}


RGB_image::~RGB_image() {
    delete [] this->data;
}


RGB_color RGB_image::get_pixel(int i, int j) {
    return this->data[i*this->h + j];
}


void RGB_image::set_pixel(int i, int j, RGB_color value) {
    this->data[i*this->h + j] = value;
}


void RGB_image::imwrite(std::string path) {

    float r,g,b;
    int index;
    float* data_float = new float[this->h*this->w*3];

    for (int i = 0; i < this->w*this->h; i++) {
        index = i*3;
        b = this->data[i].value[0];
        g = this->data[i].value[1];
        r = this->data[i].value[2];
        data_float[index] = r;
        data_float[index+1] = g;
        data_float[index+2] = b;
    }

    cv::Mat image(this->h, this->w, CV_32FC3, data_float);
    cv::imwrite(path, image);
    delete [] data_float;
}


Ray::Ray(float x, float y, float z, float d_x, float d_y, float d_z):origin(x,y,z), direction_vec(d_x, d_y, d_z){}


Sphere::Sphere(float center_x, float center_y, float center_z, float radius, RGB_color color):center(center_x,center_y,center_z),color(color){
    this->radius = radius;
}


Scene::Scene(LightSource light, std::vector<Sphere> spheres, Plane ground):light(light),ground(ground),spheres(spheres){}



bool hit_check(Ray ray, Scene scene, Vector3D& hit_point, Vector3D& hit_normal, RGB_color& hit_color, bool& is_reflective) {

    float A, B, C, disc_2, t_1, t_2;
    float current_hit_coef = std::numeric_limits<float>::infinity();
    bool found = false;

    int num_sphere = scene.spheres.size();
    std::vector<Sphere> spheres = scene.spheres;

    // Sphere hit check

    is_reflective = false;

    for (int i = 0; i < num_sphere; i++) {

        Sphere sphere = spheres.at(i);
        Vector3D p = ray.origin - sphere.center;
        A = ray.direction_vec.dot(ray.direction_vec);
        B = ray.direction_vec.multiply(2).dot(p);
        C = (p).dot(p) - sphere.radius * sphere.radius;

        disc_2 = B * B - 4 * A * C;

        if (disc_2 <= 0)
            continue;

        t_1 = (-B + sqrt(disc_2)) / (2 * A);
        t_2 = (-B - sqrt(disc_2)) / (2 * A);

        if (t_1 < t_2 && t_1 > 0 && t_1 < current_hit_coef) {
            found = true;
            hit_color = sphere.color;
            current_hit_coef = t_1;
            hit_point = ray.origin + ray.direction_vec.multiply(current_hit_coef);
            hit_normal = hit_point-sphere.center;

        } else if (t_2 < t_1 && t_2 > 0 && t_2 < current_hit_coef) {
            found = true;
            hit_color = sphere.color;
            current_hit_coef = t_2;
            hit_point = ray.origin + ray.direction_vec.multiply(current_hit_coef);
            hit_normal = hit_point-sphere.center;
        }
    }


    // Plane hit check

    float t;
    float dot = scene.ground.unit_normal.dot(ray.direction_vec);

    if (abs(dot) > 0.0001f){

        t = (scene.ground.point - ray.origin).dot(scene.ground.unit_normal) / dot;

        if (t > 0 && t < current_hit_coef){
            is_reflective = true;
            found = true;
            hit_color = scene.ground.color;
            current_hit_coef = t;
            hit_point = ray.origin + ray.direction_vec.multiply(current_hit_coef);
            hit_normal = scene.ground.unit_normal;
        }
    }

    return found;

}


RGB_color calculate_diffuse(LightSource light, Vector3D hit_point, Vector3D hit_normal, RGB_color hit_point_color){
	
	Vector3D l = light.center - hit_point;
    float r = light.center.distance_between(hit_point);

	float coef = std::max((float)0, l.cosine_between(hit_normal));

	return RGB_color(light.red_intensity*coef*hit_point_color.value[0],
					 light.green_intensity*coef*hit_point_color.value[1],
					 light.blue_intensity*coef*hit_point_color.value[2]);

}
