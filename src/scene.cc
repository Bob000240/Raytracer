#include "scene.h"
#include "intersections.h"
#include <cmath>

bool Scene::intersect(const Ray &ray, HitRecord &rec) const
{
    const double INF = 0x3f3f3f3f;
    double closestT = INF;
    bool hit = false;

    for (const sphere &s : spheres)
    {
        double t;
        if (hitSphere(s, ray, t) && t < closestT)
        {
            closestT = t;
            sphereHit(s, ray, t, rec);
            hit = true;
        }
    }

    for (const cone &c : cones)
    {
        double t;
        if (hitCone(c, ray, t) && t < closestT)
        {
            closestT = t;
            coneHit(c, ray, t, rec);
            hit = true;
        }
    }

    for (const cylinder &c : cylinders)
    {
        double t;
        if (hitCylinder(c, ray, t) && t < closestT)
        {
            closestT = t;
            cylinderHit(c, ray, t, rec);
            hit = true;
        }
    }

    for (const triangle &tri : triangles)
    {
        double t, a, b, c;
        if (hitTriangle(tri, ray, t, a, b, c) && t < closestT)
        {
            closestT = t;
            triangleHit(tri, ray, t, a, b, c, rec);
            hit = true;
        }
    }

    return hit;
}

void Scene::sphereHit(const sphere &s, const Ray &ray, double t, HitRecord &rec) const
{
    rec.t        = t;
    rec.Od       = s.mat.diffuse;
    rec.Os       = s.mat.specular;
    rec.ka       = s.mat.ka;
    rec.kd       = s.mat.kd;
    rec.ks       = s.mat.ks;
    rec.shininess = s.mat.shininess;
    rec.alpha    = s.mat.alpha;
    rec.eta      = s.mat.eta;

    rec.hitPoint = ray.orig + ray.direc * t;
    rec.N        = (rec.hitPoint - vec3(s.cx, s.cy, s.cz)).unit();

    if (s.mat.textureId >= 0 && s.mat.textureId < (int)textures.size())
    {
        double phi   = std::acos(rec.N.z);
        double theta = std::atan2(rec.N.y, rec.N.x);

        double v = phi / M_PI;
        double u = (theta >= 0) ? theta / (2 * M_PI)
                                : (theta + 2 * M_PI) / (2 * M_PI);

        rec.Od = nearestNeighbor(textures[s.mat.textureId], u, v);
    }
}

void Scene::coneHit(const cone &s, const Ray &ray, double t, HitRecord &rec) const
{
    rec.t        = t;
    rec.Od       = s.mat.diffuse;
    rec.Os       = s.mat.specular;
    rec.ka       = s.mat.ka;
    rec.kd       = s.mat.kd;
    rec.ks       = s.mat.ks;
    rec.shininess = s.mat.shininess;
    rec.alpha    = s.mat.alpha;
    rec.eta      = s.mat.eta;

    rec.hitPoint = ray.orig + ray.direc * t;

    vec3 tip(s.cx, s.cy, s.cz);
    vec3 axis = vec3(s.dx, s.dy, s.dz).unit();

    double theta = s.angle * M_PI / 180;
    double cosT  = std::cos(theta);
    double cos2  = cosT * cosT;

    vec3   q = rec.hitPoint - tip;
    double m = q.dot(axis);

    rec.N = (axis * m - q * cos2).unit();

    if (rec.N.dot(ray.direc) > 0)
        rec.N = -rec.N;

    if (s.mat.textureId >= 0 && s.mat.textureId < (int)textures.size())
    {
        vec3 C(s.cx, s.cy, s.cz);
        vec3 A  = vec3(s.dx, s.dy, s.dz).unit();
        vec3 CP = rec.hitPoint - C;

        double h = CP.dot(A);
        double v = h / s.height;

        vec3 radial = CP - A * h;

        vec3 helper = (std::fabs(A.z) < 0.999) ? vec3(0, 0, 1) : vec3(0, 1, 0);
        vec3 U  = (helper.cross(A)).unit();
        vec3 VV = (A.cross(U)).unit();

        double ang = std::atan2(radial.dot(VV), radial.dot(U));
        double u   = (ang >= 0) ? ang / (2 * M_PI)
                                : (ang + 2 * M_PI) / (2 * M_PI);

        rec.Od = nearestNeighbor(textures[s.mat.textureId], u, v);
    }
}

void Scene::cylinderHit(const cylinder &s, const Ray &ray, double t, HitRecord &rec) const
{
    rec.t        = t;
    rec.Od       = s.mat.diffuse;
    rec.Os       = s.mat.specular;
    rec.ka       = s.mat.ka;
    rec.kd       = s.mat.kd;
    rec.ks       = s.mat.ks;
    rec.shininess = s.mat.shininess;
    rec.alpha    = s.mat.alpha;
    rec.eta      = s.mat.eta;

    rec.hitPoint = ray.orig + ray.direc * t;

    vec3 base(s.cx, s.cy, s.cz);
    vec3 axis = vec3(s.dx, s.dy, s.dz).unit();

    vec3   q    = rec.hitPoint - base;
    double proj = q.dot(axis);
    vec3   radial = q - axis * proj;

    rec.N = radial.unit();

    if (s.mat.textureId >= 0 && s.mat.textureId < (int)textures.size())
    {
        vec3 C(s.cx, s.cy, s.cz);
        vec3 A  = vec3(s.dx, s.dy, s.dz).unit();
        vec3 CP = rec.hitPoint - C;

        double h = CP.dot(A);
        double v = h / s.length;

        vec3 radial2 = CP - A * h;

        vec3 helper = (std::fabs(A.z) < 0.999) ? vec3(0, 0, 1) : vec3(0, 1, 0);
        vec3 U  = (helper.cross(A)).unit();
        vec3 VV = (A.cross(U)).unit();

        double theta = std::atan2(radial2.dot(VV), radial2.dot(U));
        double u     = (theta >= 0) ? theta / (2 * M_PI)
                                    : (theta + 2 * M_PI) / (2 * M_PI);

        rec.Od = nearestNeighbor(textures[s.mat.textureId], u, v);
    }
}

void Scene::triangleHit(const triangle &tri, const Ray &ray,
                        double tHit, double a, double b, double c,
                        HitRecord &rec) const
{
    rec.t        = tHit;
    rec.Od       = tri.mat.diffuse;
    rec.Os       = tri.mat.specular;
    rec.ka       = tri.mat.ka;
    rec.kd       = tri.mat.kd;
    rec.ks       = tri.mat.ks;
    rec.shininess = tri.mat.shininess;
    rec.alpha    = tri.mat.alpha;
    rec.eta      = tri.mat.eta;

    rec.hitPoint = ray.orig + ray.direc * tHit;

    bool hasNormals = !(tri.n0.norm() == 0 && tri.n1.norm() == 0 && tri.n2.norm() == 0);

    if (!hasNormals)
    {
        vec3 e1 = tri.v1 - tri.v0;
        vec3 e2 = tri.v2 - tri.v0;
        rec.N = e1.cross(e2).unit();
    }
    else
    {
        rec.N = (tri.n0 * a + tri.n1 * b + tri.n2 * c).unit();
    }

    if (rec.N.dot(ray.direc) > 0)
        rec.N = -rec.N;

    if (tri.mat.textureId >= 0 && tri.mat.textureId < (int)textures.size())
    {
        double u = a * tri.t0.x + b * tri.t1.x + c * tri.t2.x;
        double v = a * tri.t0.y + b * tri.t1.y + c * tri.t2.y;

        rec.Od = nearestNeighbor(textures[tri.mat.textureId], u, v);
    }
}

Color Scene::nearestNeighbor(const Texture &tex, double u, double v) const
{
    if (!tex.valid())
        return Color(1, 1, 1);

    u = u - std::floor(u);
    v = v - std::floor(v);

    int i = (int)std::round(u * (tex.width - 1));
    int j = (int)std::round(v * (tex.height - 1));

    if (i < 0) i = 0;
    if (i >= tex.width)  i = tex.width  - 1;
    if (j < 0) j = 0;
    if (j >= tex.height) j = tex.height - 1;

    return tex.pixels[j * tex.width + i];
}
