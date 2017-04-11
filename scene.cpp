#include "scene.h"

using namespace std;

Ray::Ray(const Vec3f &position, const Vec3f &direction) : position(position), direction(direction) {}
Ray::Ray(const Vec3f &position, const float x, const float y, const float z) : position(position), direction(Vec3f(x, y, z)) {}

Sample::Sample(Vec3f &intersect_pos, float intersect_dist, Vec3f &normal, int primitive_id) : intersect_pos(intersect_pos), intersect_dist(intersect_dist), normal(normal), primitive_id(primitive_id) {}

Scene::Scene() : sphere_pos(0, 0, 3), sphere_rad(.5) {}

Sample Scene::intersect(const Ray &ray){
    int primitive_id = 0;
    float t = 999999.0;
    if(intersect_plane(t, ray.direction, ray.position, 2, 4)){
        primitive_id = Scene::back_plane_id;
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
            normal = (intersect_pos - Scene::sphere_pos).normalized();
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
