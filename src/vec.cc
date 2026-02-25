#include "vec.h"
#include <cmath>

vec::vec(double x, double y, double z) : x(x), y(y), z(z) {}
vec::vec(std::vector<double> v) : x(v[0]), y(v[1]), z(v[2]) {}

vec vec::operator-() const { return vec(-x, -y, -z); }
vec vec::operator-(const vec &other) const { return vec(x - other.x, y - other.y, z - other.z); }
vec vec::operator+(const vec &other) const { return vec(x + other.x, y + other.y, z + other.z); }
vec vec::operator*(double scalar) const { return vec(x * scalar, y * scalar, z * scalar); }
vec vec::operator/(double scalar) const { return vec(x / scalar, y / scalar, z / scalar); }

double vec::dot(const vec &other) const { return x * other.x + y * other.y + z * other.z; }

vec vec::cross(const vec &other) const
{
    return vec(y * other.z - z * other.y,
               z * other.x - x * other.z,
               x * other.y - y * other.x);
}

double vec::norm() const { return std::sqrt(x * x + y * y + z * z); }

vec vec::unit() const
{
    double n = norm();
    if (n == 0)
        return vec(0, 0, 0);
    return vec(x / n, y / n, z / n);
}