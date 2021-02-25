# include "Complex.hpp"
# include <math.h>

Complex::Complex(Point p)
{
    _x = p.x;
    _y = p.y;
}

Complex::Complex(float radius, float deg)
{
    _x = radius * (float)cos(deg / 180.0 * M_PI);
    _y = radius * (float)sin(deg / 180.0 * M_PI);
}

Complex::~Complex() {}

float Complex::x() const
{
    return (_x);
}

float Complex::y() const
{
    return (_y);
}

void Complex::set_x(float x)
{
    _x = x;
}

void Complex::set_y(float y)
{
    _y = y;
}

void Complex::set(float x, float y)
{
    _x = x;
    _y = y;
}

Complex operator+(const Complex &a, const Complex &b)
{
    return Complex(Point(
        a.x() + b.x(),
        a.y() + b.y()
    ));
}

Complex operator-(const Complex &a, const Complex &b)
{
    return Complex(Point(
        a.x() - b.x(),
        a.y() - b.y()
    ));
}

Complex operator*(const Complex &a, const Complex &b)
{
    return Complex(Point(
        a.x() * b.x() - a.y() * b.y(),
        a.x() * b.y() + a.y() * b.x()
    ));
}

Complex operator*(const Complex &a, float scalar)
{
    return Complex(Point(
        a.x() * scalar,
        a.y() * scalar
    ));
}

Complex operator/(const Complex &a, float scalar)
{
    return Complex(Point(
        a.x() / scalar,
        a.y() / scalar
    ));
}

float Complex::get_angle(void) const
{
    return ((float)atan2(_x, -_y) * 180 / M_PI);
}

float Complex::get_radius(void) const
{
    return ((float)sqrt(pow(_x, 2) + pow(_y, 2)));
}

float Complex::get_distance(const Complex &b) const
{
    return ((float)sqrt(pow(_x - b.x(), 2) + pow(_y - b.y(), 2)));
}

Complex Complex::stage(float stage) const
{
    float rad = get_radius();

    if (rad > stage) {
        return (*this / rad);
    }
    return (*this);
}

Complex Complex::normalize() const
{
    float rad = get_radius();

    if (rad) {
        return (*this / rad);
    }
    return (*this);
}

void operator<<(std::ostream &off, const Complex &a)
{
    if (a.y() < 0)
        off << a.x() << " " << a.y() << " i\n";
    else 
        off << a.x() << " + " << a.y() << " i\n";
}