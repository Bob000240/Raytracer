#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include "scene_types.h"

bool hitSphere(const sphere &s, const Ray &ray, double &tHit);
bool hitCylinder(const cylinder &s, const Ray &ray, double &tHit);
bool hitCone(const Cone &s, const Ray &ray, double &tHit);

#endif