#ifndef __COMPLEX_HPP__
# define __COMPLEX_HPP__

# include <math.h>

struct Point
{
    double x;
    double y;
    Point(double x, double y): x(x), y(y) {}
};

class Complex
{
public:
    Complex(Point p);
    Complex(){};
    Complex(double radius, double deg);
    double get_angle(void) const;
    double get_radius(void) const;
    double get_distance_diff(const Complex &b) const;
    Complex stage(double stage);
    Complex normalize();
    double x() const;
    double y() const;
    void set_x(double x);
    void set_y(double y);
    void set(double x, double y);
    ~Complex();
private:
    double _x;
    double _y;
};

#endif /* __COMPLEX_HPP__ */
