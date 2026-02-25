#include "intersections.h"
#include <cmath>

bool hitSphere(const sphere &s, const Ray &ray, double &tHit)
{
    vec center(s.cx, s.cy, s.cz);
    vec oc = ray.orig - center;

    double A = ray.direc.dot(ray.direc);
    double B = 2 * oc.dot(ray.direc);
    double C = oc.dot(oc) - s.radius * s.radius;

    double disc = B * B - 4 * A * C;
    if (disc < 0)
        return false;

    double sq = std::sqrt(disc);
    double t0 = (-B - sq) / (2 * A);
    double t1 = (-B + sq) / (2 * A);

    bool found = false;
    double best = 0x3f3f3f3f;

    if (t0 > 0)
    {
        best = t0;
        found = true;
    }
    if (t1 > 0 && t1 < best)
    {
        best = t1;
        found = true;
    }

    if (!found)
        return false;

    tHit = best;
    return true;
}

bool hitCylinder(const cylinder &s, const Ray &ray, double &tHit)
{
    vec base(s.cx, s.cy, s.cz);
    vec axis(s.dx, s.dy, s.dz);
    axis = axis.unit();

    vec f = ray.orig - base;
    vec d = ray.direc;

    double A = d.dot(d) - (d.dot(axis) * d.dot(axis));
    double B = 2 * (d.dot(f) - (d.dot(axis) * f.dot(axis)));
    double C = f.dot(f) - (f.dot(axis) * f.dot(axis)) - s.radius * s.radius;

    double disc = B * B - 4 * A * C;
    if (disc < 0)
        return false;

    double sq = std::sqrt(disc);
    double t0 = (-B - sq) / (2 * A);
    double t1 = (-B + sq) / (2 * A);

    bool found = false;
    double best = 0x3f3f3f3f;

    if (t0 > 0)
    {
        vec x0 = ray.orig + d * t0;
        double proj0 = (x0 - base).dot(axis);
        if (proj0 >= 0 && proj0 <= s.length)
        {
            best = t0;
            found = true;
        }
    }

    if (t1 > 0)
    {
        vec x1 = ray.orig + d * t1;
        double proj1 = (x1 - base).dot(axis);
        if (proj1 >= 0 && proj1 <= s.length && t1 < best)
        {
            best = t1;
            found = true;
        }
    }

    if (!found)
        return false;

    tHit = best;
    return true;
}

bool hitCone(const Cone &s, const Ray &ray, double &tHit)
{
    vec tip(s.cx, s.cy, s.cz);
    vec axis(s.dx, s.dy, s.dz);
    axis = axis.unit();

    vec v = ray.orig - tip;
    vec d = ray.direc;

    double theta = s.angle * M_PI / 180.0;
    double cosT = std::cos(theta);
    double cos2 = cosT * cosT;

    double da = d.dot(axis);
    double va = v.dot(axis);

    double A = da * da - cos2 * d.dot(d);
    double B = 2 * (da * va - cos2 * v.dot(d));
    double C = va * va - cos2 * v.dot(v);

    if (std::abs(A) < 1e-12)
        return false;

    double disc = B * B - 4 * A * C;
    if (disc < 0)
        return false;

    double sq = std::sqrt(disc);
    double t0 = (-B - sq) / (2 * A);
    double t1 = (-B + sq) / (2 * A);

    bool found = false;
    double best = 0x3f3f3f3f;

    if (t0 > 0)
    {
        vec x0 = ray.orig + d * t0;
        double h0 = (x0 - tip).dot(axis);
        if (h0 >= 0 && h0 <= s.height)
        {
            best = t0;
            found = true;
        }
    }

    if (t1 > 0)
    {
        vec x1 = ray.orig + d * t1;
        double h1 = (x1 - tip).dot(axis);
        if (h1 >= 0 && h1 <= s.height && t1 < best)
        {
            best = t1;
            found = true;
        }
    }

    if (!found)
        return false;

    tHit = best;
    return true;
}