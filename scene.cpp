#include "scene.h"

using namespace std;

Ray::Ray(const Vec3f &position, const Vec3f &direction) : position(position), direction(direction) {}
Ray::Ray(const Vec3f &position, const float x, const float y, const float z) : position(position), direction(Vec3f(x, y, z)) {}

Sample::Sample(Vec3f &intersect_pos, float intersect_dist, Vec3f &normal, int primitive_id) : intersect_pos(intersect_pos), intersect_dist(intersect_dist), normal(normal), primitive_id(primitive_id) {}

Scene::Scene() : sphere_pos(-0.2, -0.15, 3), sphere_rad(.5), sphere2_pos(0.4, -0.45, 2.2), sphere2_rad(.2)  {}

Sample Scene::intersect(const Ray &ray){
    int primitive_id = 0;
    float t = 999999.0;
    if(intersect_plane(t, ray.direction, ray.position, 2, 4)){
        primitive_id = Scene::back_plane_id;
    }
    if(intersect_sphere(t, ray.direction, ray.position, sphere2_pos, sphere2_rad)){
        primitive_id = sphere2_id;
    }
    if(intersect_sphere(t, ray.direction, ray.position, sphere_pos, sphere_rad)){
        primitive_id = sphere_id;
    }
    if(intersect_plane(t, ray.direction, ray.position, 1, -1)){
        primitive_id = bottom_plane_id;
    }
    if(intersect_plane(t, ray.direction, ray.position, 0, -1)){
        primitive_id = left_plane_id;
    }
    if(intersect_plane(t, ray.direction, ray.position, 0, 1)){
        primitive_id = right_plane_id;
    }
    if(intersect_plane(t, ray.direction, ray.position, 1, 1)){
        primitive_id = top_plane_id;
    }
    Vec3f intersect_pos = ray.position + (ray.direction * t);
    Vec3f normal;
    switch(primitive_id)
    {
        case sphere_id:
        {
            normal = (intersect_pos - sphere_pos).normalized();
        }
        break;
        case sphere2_id:
        {
            normal = (intersect_pos - sphere2_pos).normalized();
        }
        break;
        case top_plane_id:
        {
            normal = Vec3f(0, -1, 0);
        }
        break;
        case bottom_plane_id:
        {
            normal = Vec3f(0, 1, 0);
        }
        break;
        case left_plane_id:
        {
            normal = Vec3f(1, 0, 0);
        }
        break;
        case right_plane_id:
        {
            normal = Vec3f(-1, 0, 0);
        }
        break;
        case back_plane_id:
        {
            normal = Vec3f(0, 0, -1);
        }
        break;
        default:
        break;
    }
    return Sample(intersect_pos, t, normal, primitive_id);
}

Vec3f Scene::shade(const Ray &ray){
    Vec3f light_pos(.4, .75, 2);
    const float max_light_depth = 10.0;
    Sample result = this->intersect(ray);
    Vec3f color(0, 0, 0);
    switch(result.primitive_id)
    {
        case Scene::sphere_id:
        {
            //r=d−2(d⋅n)n
            Vec3f rayflect_dir = ray.direction - (result.normal * (ray.direction.dot(result.normal) * 2));
            Vec3f rayflect_pos = result.intersect_pos + (rayflect_dir*0.001f);
            Ray rayflect(rayflect_pos, rayflect_dir);
            color = this->shade(rayflect);
        }
        break;
        case Scene::sphere2_id:
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
    color *= light_val;

    // shadows
    Vec3f rayPos = result.intersect_pos + (light_dir*0.001f);
    Ray light_ray(rayPos, light_dir);
    Sample shadow_result = this->intersect(light_ray);
    if (shadow_result.intersect_dist < light_dist){
        color *= 0.5;
    }
    return color;
}