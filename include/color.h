#ifndef COLOR_H
#define COLOR_H

class Color
{
private:
    double r, g, b;

public:
    Color(double r = 0, double g = 0, double b = 0);

    double getR() const;
    double getG() const;
    double getB() const;

    Color &setR(double r);
    Color &setG(double g);
    Color &setB(double b);

    Color operator+(const Color &other) const;
    Color &operator+=(const Color &other);
    Color operator*(double s) const;

    Color multiply(const Color &other) const;
    Color clamped() const;

    static int to255(double x);
    int Ri() const;
    int Gi() const;
    int Bi() const;
};

#endif