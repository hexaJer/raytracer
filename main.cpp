#include <iostream>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "scene.h"
#include "vector.h"
#include "intersect.h"

int main(){
    char const *filename = "out.png";
    const int width = 256;
    const int height = 256;
    const int comp = 3;
    const float max_light_depth = 10.0;
    Vec3f v_null(0, 0, 0);
    Vec3f light_pos(0, .75, 2);
    Scene scene;

    char data[width * height * comp] = {0};

    for(int x=0; x < width; x++){
        for(int y=0; y < height; y++){
            float rx = float(x) / 255.0 - .5;
            float ry = float(y) / 255.0 - .5;
            ry *= -1;
            float rz = 1.0;
            Vec3f v_dir(rx, ry, rz);
            v_dir = v_dir.normalized();
            Ray ray(v_null, v_dir);
            Sample result = scene.intersect(ray);
            Vec3f color(0, 0, 0);
            switch(result.primitive_id)
            {
                case Scene::sphere_id:
                {
                    color.r = 0.8;
                    color.g = 0.8;
                    color.b = 0.8;
                }
                break;
                case Scene::top_plane_id:
                {
                    color.r = 1;
                    color.g = 0;
                    color.b = 0;
                }
                break;
                case Scene::bottom_plane_id:
                {
                    color.r = 1;
                    color.g = 1;
                    color.b = 0;
                }
                break;
                case Scene::left_plane_id:
                {
                    color.r = 1;
                    color.g = 0;
                    color.b = 1;
                }
                break;
                case Scene::right_plane_id:
                {
                    color.r = 0;
                    color.g = 0;
                    color.b = 1;
                }
                break;
                case Scene::back_plane_id:
                {
                    color.r = 0;
                    color.g = 1;
                    color.b = 1;
                }
                break;
                default:
                break;
            }
            Vec3f light_dir = light_pos - result.intersect_pos;
            light_dir = light_dir.normalized();
            float light_val = (light_dir.dot(result.normal) + 1) / 2;
            float light_dist = (light_pos - result.intersect_pos).mag();
            light_val *= (1 - (light_dist / max_light_depth));
            if (light_val < 0){
                light_val = 0;
            }
            int i = ((comp * width) * y) + (comp * x);
            data[i + 0] = color.r * 255 * light_val;
            data[i + 1] = color.g * 255 * light_val;
            data[i + 2] = color.b * 255 * light_val;
        }
    }
    int res = stbi_write_png(filename, width, height, comp, data, width * comp);
    return 0;
}
