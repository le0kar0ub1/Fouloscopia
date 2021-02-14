#ifndef __COMPLEX_HPP__
# define __COMPLEX_HPP__

# include <math.h>
# include <iostream>

struct Point
{
    float x;
    float y;
    Point(float x, float y): x(x), y(y) {}
};

class Complex
{
public:
    Complex(Point p);
    Complex(): _x(0), _y(0) {};
    Complex(float radius, float deg);
    float get_angle(void) const;
    float get_radius(void) const;
    float get_distance(const Complex &b) const;
    Complex stage(float stage) const;
    Complex normalize() const;
    float x() const;
    float y() const;
    void set_x(float x);
    void set_y(float y);
    void set(float x, float y);
    ~Complex();
private:
    float _x;
    float _y;
};

Complex operator+(const Complex &a, const Complex &b);
Complex operator-(const Complex &a, const Complex &b);
Complex operator*(const Complex &a, const Complex &b);
Complex operator*(const Complex &a, float scalar);
Complex operator/(const Complex &a, float scalar);
void operator<<(std::ostream &off, const Complex &a);

#endif /* __COMPLEX_HPP__ */