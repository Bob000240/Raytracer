#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <string>
#include "data.h"

class RayTracer
{
private:
    Data data;

    // camera frame
    vec u, v, w;
    vec n;

    // image window params
    int imgW = 0, imgH = 0;
    double aspect = 0;

    double thetaH = 0;
    double thetaW = 0;
    double d = 0;
    double viewingWindowH = 0;
    double viewingWindowW = 0;

    vec upperLeft, upperRight, lowerLeft, lowerRight;
    vec delta_h, delta_v;
    vec delta_c_h, delta_c_v;

    vec viewingRay(int i, int j) const;

    Color shadeSphere(const sphere &s, const Ray &ray, double t) const;
    Color shadeCylinder(const cylinder &s, const Ray &ray, double t) const;
    Color shadeCone(const Cone &s, const Ray &ray, double t) const;

public:
    RayTracer(const std::string &filename);
    void renderShapes();
    bool shadowFlag(const vec &hitPoint, const Light &light, const vec &N) const;
    Color blinnPhong(
        const vec &hitPoint,
        const vec &N,
        const vec &V,
        const Color &Od,
        const Color &Os,
        double ka,
        double kd,
        double ks,
        int shininess) const;
};

#endif