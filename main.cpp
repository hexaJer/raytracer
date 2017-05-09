#include <iostream>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "scene.h"
#include "vector.h"
#include "intersect.h"

int main(){
    char const *filename = "out.png";
    const int width = 512;
    const int height = 512;
    const int comp = 3;
    Vec3f v_null(0, 0, 0);
    Scene scene;

    char data[width * height * comp] = {0};

    for(int x=0; x < width; x++){
        for(int y=0; y < height; y++){
            float rx = float(x) / float(width) - .5;
            float ry = float(y) / float(height) - .5;
            ry *= -1;
            float rz = 1.0;
            Vec3f v_dir(rx, ry, rz);
            v_dir = v_dir.normalized();
            Ray ray(v_null, v_dir);
            Vec3f color = scene.shade(ray);
            int i = ((comp * width) * y) + (comp * x);
            data[i + 0] = color.r * 255;
            data[i + 1] = color.g * 255;
            data[i + 2] = color.b * 255;
        }
    }
    int res = stbi_write_png(filename, width, height, comp, data, width * comp);
    return 0;
}
