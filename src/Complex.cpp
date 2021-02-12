# include "Complex.hpp"
# include <math.h>

Complex::Complex(Point p)
{
    this->_x = p.x;
    this->_y = p.y;
}

Complex::Complex(double radius, double deg)
{
    this->_x = radius * (double)cos(deg / 180.0 * M_PI);
    this->_y = radius * (double)sin(deg / 180.0 * M_PI);
}

Complex::~Complex() {}

double Complex::x() const
{
    return (_x);
}

double Complex::y() const
{
    return (_y);
}

void Complex::set_x(double x)
{
    _x = x;
}

void Complex::set_y(double y)
{
    _y = y;
}

void Complex::set(double x, double y)
{
    _x = x;
    _y = y;
}

Complex operator+(const Complex &a, const Complex &b)
{
    return Complex(
        a.x() + b.x(),
        a.y() + b.y()
    );
}

Complex operator-(const Complex &a, const Complex &b)
{
    return Complex(
        a.x() - b.x(),
        a.y() - b.y()
    );
}

Complex operator*(const Complex &a, const Complex &b)
{
    return Complex(
        a.x() * b.x() - a.y() * b.y(),
        a.x() * b.y() + a.y() * b.x()
    );
}

Complex operator*(const Complex &a, double scalar)
{
    return Complex(
        a.x() * scalar,
        a.y() * scalar
    );
}

Complex operator/(const Complex &a, double scalar)
{
    return Complex(
        a.x() / scalar,
        a.y() / scalar
    );
}

double Complex::get_angle(void) const
{
    return (atan2(_x, -_y) * 180 / M_PI);
}

double Complex::get_radius(void) const
{
    return (sqrt(pow(_x, 2) + pow(_y, 2)));
}

double Complex::get_distance_diff(const Complex &b) const
{
    return (sqrt(pow(_x - b.x(), 2) + pow(_y - b.y(), 2)));
}

Complex Complex::stage(double stage)
{
    double cur = get_radius();

    if (cur > stage) {
        return (*this / cur);
    }
    return (*this);
}

Complex Complex::normalize()
{
    double rad = get_radius();

    if (rad) {
        return (Complex(
            _x /rad,
            _y /rad
        ));
    }
    return (*this);
}