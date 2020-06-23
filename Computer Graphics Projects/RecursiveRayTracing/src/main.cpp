#include "Render.h"
#include <fstream>

#define MAX_DEPTH 5

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
        RGB_color color(r,g,b);
        Sphere sphere(x,y,z,radius,color);
        spheres.push_back(sphere);
    }


    Camera camera;

    RGB_image image(1000, 1000);

    float red_light_intensity = 1;
    float green_light_intensity = 1;
    float blue_light_intensity = 1;

    LightSource light(500,500,500,red_light_intensity,green_light_intensity,blue_light_intensity);

    Plane ground(Vector3D(0,1,0), Vector3D(0,-50,0), RGB_color(150,150,150));
    Scene scene(light, spheres, ground);


    for (int j = 999; j >= 0; j--) {
        for (int i = 0; i < 1000; i++) {

            RGB_color color(0,0,0);
            float depth = 0;
            Ray ray = camera.generate_ray(i, j, 1000, 1000, 100, 100, 100);
            cast_ray(ray, scene, MAX_DEPTH, color, depth);

            if (depth)
                image.set_pixel(999 - j, i, color.divide(depth));

        }
    }

    image.imwrite("output.jpg");

    return 0;
}