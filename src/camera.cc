#include "camera.h"
#include <cmath>

Camera::Camera(const vec3 &eye, const vec3 &viewdir, const vec3 &updir,
               int vfov, int imgW, int imgH)
    : eyePos(eye), imgW(imgW), imgH(imgH)
{
    w = -viewdir.unit();
    u = viewdir.cross(updir).unit();
    v = u.cross(viewdir).unit();
    n = viewdir.unit();

    double aspect = (double)imgW / (double)imgH;
    double thetaH = (double)vfov * M_PI / 180.0;
    double thetaW = 2 * std::atan(aspect * std::tan(thetaH / 2));

    double d = imgH * 0.5 / std::tan(0.5 * thetaH);

    double viewingWindowH = 2 * d * std::tan(0.5 * thetaH);
    double viewingWindowW = 2 * d * std::tan(0.5 * thetaW);

    upperLeft = eye + n * d - u * (viewingWindowW / 2) + v * (viewingWindowH / 2);
    vec3 upperRight = eye + n * d + u * (viewingWindowW / 2) + v * (viewingWindowH / 2);
    vec3 lowerLeft  = eye + n * d - u * (viewingWindowW / 2) - v * (viewingWindowH / 2);

    delta_h = (upperRight - upperLeft) / (double)imgW;
    delta_v = (lowerLeft  - upperLeft) / (double)imgH;

    delta_c_h = delta_h * 0.5;
    delta_c_v = delta_v * 0.5;
}

Ray Camera::generateRay(int i, int j) const
{
    vec3 p = upperLeft + delta_h * (double)i + delta_v * (double)j + delta_c_h + delta_c_v;
    return Ray{eyePos, (p - eyePos).unit()};
}

Ray Camera::generateRay(int i, int j, double ox, double oy) const
{
    vec3 p = upperLeft + delta_h * ((double)i + ox) + delta_v * ((double)j + oy) + delta_c_h + delta_c_v;
    return Ray{eyePos, (p - eyePos).unit()};
}
