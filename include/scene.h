#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "scene_types.h"

class Scene
{
public:
    std::vector<sphere>   spheres;
    std::vector<cone>     cones;
    std::vector<cylinder> cylinders;
    std::vector<triangle> triangles;
    std::vector<Light>    lights;
    std::vector<Texture>  textures;
    Color  bkgcolor{0, 0, 0};
    double bgdIoRefraction = 1.0;

    bool intersect(const Ray &ray, HitRecord &rec) const;

private:
    void sphereHit  (const sphere   &s,   const Ray &ray, double t,
                     HitRecord &rec) const;
    void coneHit    (const cone     &c,   const Ray &ray, double t,
                     HitRecord &rec) const;
    void cylinderHit(const cylinder &c,   const Ray &ray, double t,
                     HitRecord &rec) const;
    void triangleHit(const triangle &tri, const Ray &ray, double tHit,
                     double a, double b, double c, HitRecord &rec) const;
    Color nearestNeighbor(const Texture &tex, double u, double v) const;
};

#endif
