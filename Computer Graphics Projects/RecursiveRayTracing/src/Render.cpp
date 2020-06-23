//
// Created by Alperen Bağ on 10.04.2020.
//

#include "Render.h"

void cast_ray(Ray ray, Scene scene, int MAX_DEPTH, RGB_color& color, float& depth){

    if (depth >= MAX_DEPTH)
        return;

    Vector3D hit_normal(0,0,0);

    Vector3D hit_point(0,0,0);
    Vector3D light_hit_point(0,0,0);

    RGB_color hit_point_color(0,0,0);
    RGB_color hit_point_color_dummy(0,0,0);

    bool is_reflective, is_reflective_dummy;

    if (hit_check(ray, scene, hit_point, hit_normal, hit_point_color, is_reflective)) {

        // Camera sees an object
        depth += 1;

        RGB_color ambient_color = hit_point_color.divide(10);
        RGB_color diffuse_color(0,0,0);


        Ray new_ray(hit_point.coordinate[0], hit_point.coordinate[1], hit_point.coordinate[2], 0, 0, 0);

        ray.direction_vec = ray.direction_vec.divide(ray.direction_vec.length);
        hit_normal = hit_normal.divide(hit_normal.length);
        new_ray.direction_vec = ray.direction_vec - hit_normal.multiply(ray.direction_vec.dot(hit_normal)*2);
        new_ray.origin = new_ray.origin + new_ray.direction_vec.multiply(0.0001f);

        Vector3D light_2_hit_point_vec = hit_point - scene.light.center;
        Ray light_2_hit_point(scene.light.center.coordinate[0], scene.light.center.coordinate[1],
                              scene.light.center.coordinate[2],
                              light_2_hit_point_vec.coordinate[0], light_2_hit_point_vec.coordinate[1],
                              light_2_hit_point_vec.coordinate[2]);


        hit_check(light_2_hit_point, scene, light_hit_point, hit_normal, hit_point_color_dummy, is_reflective_dummy);

        Vector3D diff = hit_point - light_hit_point;

        if (abs(diff.coordinate[0]) < 0.1 && abs(diff.coordinate[1]) < 0.1 && abs(diff.coordinate[2]) < 0.1) {

            // Light source hits the point

            diffuse_color = calculate_diffuse(scene.light, light_hit_point, hit_normal, hit_point_color);

        } else {
            // Light source does not hit the point
        }


        color = color + diffuse_color + ambient_color;

        // cast_ray again if the object is reflective
        if (is_reflective) {
            cast_ray(new_ray, scene, MAX_DEPTH, color, depth);
        }

    } else {

        // No object on the ray's route
    }
}
