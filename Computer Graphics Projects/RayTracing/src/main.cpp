#include "Camera.h"
#include <fstream>

int main() {

    int num_sphere;
    float x,y,z,r,g,b,radius;
    std::vector<Sphere> spheres;

    std::cout<<"Number of spheres"<<std::endl;
    std::cin >> num_sphere;

    for (int i = 0; i < num_sphere; i++) {
        std::cout<<"Coordinate of sphere (input x y z coordinates with leaving one character space between them)"<<std::endl;
        std::cin >> x >> y >> z;
        std::cout<<"Radius of the sphere"<<std::endl;
        std::cin >> radius;
        std::cout<<"Color of sphere (input RGB values with leaving one character space between values)"<<std::endl;
        std::cin >> r >> g >> b;
        RGB_pixel color(r,g,b);
        Sphere sphere(x,y,z,radius,color);
        spheres.push_back(sphere);
    }


    Camera camera;

    RGB_image image(1000, 1000);

    Vector3D hit_point(0,0,0);
    Vector3D light_hit_point(0,0,0);

    RGB_pixel hit_point_color(0,0,0);
    RGB_pixel hit_point_color_dummy(0,0,0);

    LightSource light(500,500,500);


    for (int j = 999; j >= 0; j--) {
         for(int i = 0; i < 1000; i++) {

            Ray ray = camera.generate_ray(i, j, 1000, 1000, 100, 100, 100);

            if (hit_check(ray, spheres, num_sphere, hit_point, hit_point_color)) {

                Vector3D light_2_hit_point_vec = hit_point - light.center;
                Ray light_2_hit_point(light.center.coordinate[0], light.center.coordinate[1], light.center.coordinate[2],
                        light_2_hit_point_vec.coordinate[0], light_2_hit_point_vec.coordinate[1], light_2_hit_point_vec.coordinate[2]);

                if (hit_check(light_2_hit_point, spheres, num_sphere, light_hit_point, hit_point_color_dummy)) {

                    Vector3D diff = hit_point - light_hit_point;

                    if (abs(diff.coordinate[0]) < 0.1 && abs(diff.coordinate[1]) < 0.1 && abs(diff.coordinate[2]) < 0.1) {
                        image.set_pixel(999-j, i, hit_point_color);
                    } else {
                        hit_point_color = hit_point_color.divide(10);
                        image.set_pixel(999-j, i, hit_point_color);
                    }
                } else {
                    hit_point_color = hit_point_color.divide(10);
                    image.set_pixel(999-j, i, hit_point_color);
                }

            } else {
                continue;
            }
        }
    }

    image.imwrite("output.jpg");

    return 0;
}