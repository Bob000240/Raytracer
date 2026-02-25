#include "color.h"

Color::Color(double r, double g, double b) : r(r), g(g), b(b) {}

double Color::getR() const { return r; }
double Color::getG() const { return g; }
double Color::getB() const { return b; }

Color &Color::setR(double r)
{
    this->r = r;
    return *this;
}
Color &Color::setG(double g)
{
    this->g = g;
    return *this;
}
Color &Color::setB(double b)
{
    this->b = b;
    return *this;
}

Color Color::operator+(const Color &other) const
{
    return Color(r + other.r, g + other.g, b + other.b);
}

Color &Color::operator+=(const Color &other)
{
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

Color Color::operator*(double s) const
{
    return Color(r * s, g * s, b * s);
}

Color Color::multiply(const Color &other) const
{
    return Color(r * other.r, g * other.g, b * other.b);
}

Color Color::clamped() const
{
    double rr = r, gg = g, bb = b;
    if (rr < 0)
        rr = 0;
    if (rr > 1)
        rr = 1;
    if (gg < 0)
        gg = 0;
    if (gg > 1)
        gg = 1;
    if (bb < 0)
        bb = 0;
    if (bb > 1)
        bb = 1;
    return Color(rr, gg, bb);
}

int Color::to255(double x)
{
    if (x < 0)
        x = 0;
    if (x > 1)
        x = 1;
    return (int)(255 * x);
}

int Color::Ri() const { return to255(r); }
int Color::Gi() const { return to255(g); }
int Color::Bi() const { return to255(b); }