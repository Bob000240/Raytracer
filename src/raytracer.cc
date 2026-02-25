#include "raytracer.h"
#include "intersections.h"

#include <cstdio>
#include <cmath>
#include <iostream>

RayTracer::RayTracer(const std::string &filename) : data(filename)
{
    if (data.eye.size() < 3 || data.viewdir.size() < 3 || data.updir.size() < 3 || data.imsize.size() < 2)
    {
        std::cout << "Need proper input data\n";
        return;
    }

    w = -vec(data.viewdir).unit();
    u = vec(data.viewdir).cross(vec(data.updir)).unit();
    v = u.cross(vec(data.viewdir)).unit();

    n = vec(data.viewdir).unit();

    imgW = data.imsize[0];
    imgH = data.imsize[1];
    aspect = (double)imgW / (double)imgH;

    thetaH = (double)data.vfov * M_PI / 180;
    thetaW = 2 * std::atan(aspect * std::tan(thetaH / 2));

    d = imgH * 0.5 / std::tan(0.5 * thetaH);

    viewingWindowH = 2 * d * std::tan(0.5 * thetaH);
    viewingWindowW = 2 * d * std::tan(0.5 * thetaW);

    vec eye = vec(data.eye);
    upperLeft = eye + n * d - u * (viewingWindowW / 2) + v * (viewingWindowH / 2);
    upperRight = eye + n * d + u * (viewingWindowW / 2) + v * (viewingWindowH / 2);
    lowerLeft = eye + n * d - u * (viewingWindowW / 2) - v * (viewingWindowH / 2);
    lowerRight = eye + n * d + u * (viewingWindowW / 2) - v * (viewingWindowH / 2);

    delta_h = (upperRight - upperLeft) / (double)imgW;
    delta_v = (lowerLeft - upperLeft) / (double)imgH;

    delta_c_h = delta_h * 0.5;
    delta_c_v = delta_v * 0.5;
}

vec RayTracer::viewingRay(int i, int j) const
{
    return upperLeft + delta_h * (double)i + delta_v * (double)j + delta_c_h + delta_c_v;
}

bool RayTracer::shadowFlag(const vec &hitPoint, const Light &light, const vec &N) const
{
    vec L;
    double maxT;
    const double EPS = 1e-6;
    if (light.w == 1)
    {
        vec toLight = vec(light.x, light.y, light.z) - hitPoint;
        maxT = toLight.norm();
        L = toLight.unit();
    }
    else
    {
        maxT = 0x3f3f3f3f;
        L = vec(light.x, light.y, light.z).unit();
    }

    Ray shadowRay{hitPoint + N * EPS, L};

    for (const sphere &s : data.spheres)
    {
        double t;
        if (hitSphere(s, shadowRay, t) && t > EPS && t < maxT - EPS)
            return true;
    }

    for (const Cone &c : data.cone)
    {
        double t;
        if (hitCone(c, shadowRay, t) && t > EPS && t < maxT - EPS)
            return true;
    }

    for (const cylinder &c : data.cylinders)
    {
        double t;
        if (hitCylinder(c, shadowRay, t) && t > EPS && t < maxT - EPS)
            return true;
    }

    return false;
}

Color RayTracer::blinnPhong(
    const vec &hitPoint,
    const vec &N,
    const vec &V,
    const Color &Od,
    const Color &Os,
    double ka,
    double kd,
    double ks,
    int shininess) const
{
    Color finalColor = Od * ka;

    for (const Light &light : data.lights)
    {
        if (shadowFlag(hitPoint, light, N))
            continue;

        vec L;

        if (light.w == 1)
        {
            vec toLight = vec(light.x, light.y, light.z) - hitPoint;
            double dist = toLight.norm();
            L = toLight.unit();
        }
        else
        {
            L = vec(light.x, light.y, light.z).unit();
        }

        double ndotl = N.dot(L);
        if (ndotl < 0)
            ndotl = 0;

        Color diffuse(0, 0, 0);
        Color specular(0, 0, 0);

        if (ndotl > 0)
        {
            diffuse = Od * (kd * ndotl);

            vec H = (L + V).unit();
            double ndoth = N.dot(H);
            if (ndoth < 0)
                ndoth = 0;

            double specPow = (shininess > 0) ? std::pow(ndoth, shininess) : 0;
            specular = Os * (ks * specPow);
        }

        finalColor += (diffuse + specular) * light.intensity;
    }

    return finalColor.clamped();
}

Color RayTracer::shadeSphere(const sphere &s, const Ray &ray, double t) const
{
    Color Od = s.diffuseLight;
    Color Os = s.specularLight;
    double ka = s.coefficients[0];
    double kd = s.coefficients[1];
    double ks = s.coefficients[2];
    int shininess = s.shininess;

    vec hitPoint = ray.orig + ray.direc * t;
    vec N = (hitPoint - vec(s.cx, s.cy, s.cz)).unit();
    vec V = (vec(data.eye) - hitPoint).unit();

    return blinnPhong(hitPoint, N, V, Od, Os, ka, kd, ks, shininess);
}

Color RayTracer::shadeCylinder(const cylinder &s, const Ray &ray, double t) const
{
    Color Od = s.diffuseLight;
    Color Os = s.specularLight;
    double ka = s.coefficients[0];
    double kd = s.coefficients[1];
    double ks = s.coefficients[2];
    int shininess = s.shininess;

    vec hitPoint = ray.orig + ray.direc * t; // x

    vec base(s.cx, s.cy, s.cz);
    vec axis(s.dx, s.dy, s.dz);
    axis = axis.unit();

    vec q = hitPoint - base;
    double proj = q.dot(axis);
    vec radial = q - axis * proj;

    vec N = radial.unit();
    vec V = (vec(data.eye) - hitPoint).unit();

    return blinnPhong(hitPoint, N, V, Od, Os, ka, kd, ks, shininess);
}

Color RayTracer::shadeCone(const Cone &s, const Ray &ray, double t) const
{
    Color Od = s.diffuseLight;
    Color Os = s.specularLight;
    double ka = s.coefficients[0];
    double kd = s.coefficients[1];
    double ks = s.coefficients[2];
    int shininess = s.shininess;

    vec hitPoint = ray.orig + ray.direc * t;

    vec tip(s.cx, s.cy, s.cz);
    vec axis(s.dx, s.dy, s.dz);
    axis = axis.unit();

    double theta = s.angle * M_PI / 180;
    double cosT = std::cos(theta);
    double cos2 = cosT * cosT;

    vec q = hitPoint - tip;
    double m = q.dot(axis);

    vec N = (axis * m - q * cos2).unit();

    if (N.dot(ray.direc) > 0)
        N = -N;

    vec V = (vec(data.eye) - hitPoint).unit();

    return blinnPhong(hitPoint, N, V, Od, Os, ka, kd, ks, shininess);
}

void RayTracer::renderShapes()
{
    int W = data.imsize[0];
    int H = data.imsize[1];

    std::string outFile = outputFileName(data.getFilename());
    FILE *out = std::fopen(outFile.c_str(), "w");
    if (!out)
    {
        std::cout << "Failed to open output file\n";
        return;
    }

    std::fprintf(out, "P3\n%d %d\n255\n", W, H);

    for (int j = 0; j < H; ++j)
    {
        for (int i = 0; i < W; ++i)
        {
            vec p = viewingRay(i, j);
            Ray ray{vec(data.eye), (p - vec(data.eye)).unit()};

            Color pixelColor = data.bkgcolor;
            double closestT = 0x3f3f3f3f;

            const sphere *hitSph = nullptr;
            double hitSphT = 0;
            const Cone *hitCon = nullptr;
            double hitConeT = 0;
            const cylinder *hitCyl = nullptr;
            double hitCylT = 0;

            for (const sphere &s : data.spheres)
            {
                double t;
                if (hitSphere(s, ray, t) && t < closestT)
                {
                    closestT = t;
                    hitSph = &s;
                    hitSphT = t;
                    pixelColor = s.diffuseLight;
                }
            }

            for (const Cone &c : data.cone)
            {
                double t;
                if (hitCone(c, ray, t) && t < closestT)
                {
                    closestT = t;
                    pixelColor = c.diffuseLight;
                    hitCon = &c;
                    hitConeT = t;
                }
            }

            for (const cylinder &c : data.cylinders)
            {
                double t;
                if (hitCylinder(c, ray, t) && t < closestT)
                {
                    closestT = t;
                    pixelColor = c.diffuseLight;
                    hitCyl = &c;
                    hitCylT = t;
                }
            }

            if (hitSph != nullptr && hitSphT == closestT)
                pixelColor = shadeSphere(*hitSph, ray, hitSphT);
            else if (hitCon != nullptr && hitConeT == closestT)
                pixelColor = shadeCone(*hitCon, ray, hitConeT);
            else if (hitCyl != nullptr && hitCylT == closestT)
                pixelColor = shadeCylinder(*hitCyl, ray, hitCylT);

            std::fprintf(out, "%d %d %d ", pixelColor.Ri(), pixelColor.Gi(), pixelColor.Bi());
        }
        std::fprintf(out, "\n");
    }

    std::fclose(out);
}