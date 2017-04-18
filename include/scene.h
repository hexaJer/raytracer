#ifndef DEF_SCENE
#define DEF_SCENE

#include "vector.h"
#include "intersect.h"

class Ray
{
    public:
    Ray(const Vec3f &position, const Vec3f &direction);
    Ray(const Vec3f &position, const float x, const float y, const float z);

    const Vec3f position;
    const Vec3f direction;
    
};


class Sample
{
    public:
    Sample(Vec3f &intersect_pos, float intersect_dist, Vec3f &normal, int primitive_id);
    const Vec3f intersect_pos;
    const float intersect_dist;
    const Vec3f normal;
    const int primitive_id;
};

class Scene
{
    public:
    Scene();
    enum {
        sphere_id = 1,
        bottom_plane_id = 2,
        left_plane_id = 3,
        right_plane_id = 4,
        top_plane_id = 5,
        back_plane_id = 6,
        sphere2_id = 7,
    };
    const Vec3f sphere_pos;
    const float sphere_rad;
    const Vec3f sphere2_pos;
    const float sphere2_rad;

    Sample intersect(const Ray &ray);
    Vec3f shade(const Ray &ray);
};

#endif
