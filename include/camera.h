#ifndef CAMERA_H
#define CAMERA_H

#include "scene_types.h"

class Camera
{
public:
    Camera() = default;
    Camera(const vec3 &eye, const vec3 &viewdir, const vec3 &updir,
           int vfov, int imgW, int imgH);

    Ray generateRay(int i, int j) const;
    int width() const { return imgW; }
    int height() const { return imgH; }
    vec3 getEye() const { return eyePos; }

private:
    vec3 eyePos;
    vec3 u, v, w, n;
    int imgW = 0, imgH = 0;
    vec3 upperLeft;
    vec3 delta_h, delta_v;
    vec3 delta_c_h, delta_c_v;
};

#endif
