#ifndef VEC_H
#define VEC_H

#include <vector>

class vec
{
private:
    double x, y, z;

public:
    vec(double x = 0, double y = 0, double z = 0);
    vec(std::vector<double> v);

    vec operator-() const;
    vec operator-(const vec &other) const;
    vec operator+(const vec &other) const;
    vec operator*(double scalar) const;
    vec operator/(double scalar) const;

    double dot(const vec &other) const;
    vec cross(const vec &other) const;
    double norm() const;
    vec unit() const;
};

#endif