# include "Complex.hpp"

Complex operator+(Complex a, Complex b)
{
    a.x += b.x;
    a.y += b.y;
    return (a);
}

Complex operator-(Complex a, Complex b)
{
    a.x -= b.x;
    a.y -= b.y;
    return (a);
}

Complex operator*(Complex a, Complex b)
{
    Complex ret = {
        .x = a.x * b.x - a.y * b.y,
        .y = a.x * b.y + a.y * b.x
    };
    return (ret);
}

Complex operator*(Complex a, float scalar)
{
    a.x = a.x * scalar;
    a.y = a.y * scalar;
    return (a);
}

Complex operator/(Complex a, float scalar)
{
    Complex ret = {
        .x = a.x / scalar,
        .y = a.y / scalar
    };
    return (ret);
}

Complex complex_init_cartesian(float x, float y)
{
    Complex ini = {
        .x = x,
        .y = y
    };
    return (ini);
}

Complex complex_init_polar(float radius, float deg)
{
    Complex ini = {
        .x = radius * (float)cos(deg / 180.0 * M_PI),
        .y = radius * (float)sin(deg / 180.0 * M_PI)
    };
    return (ini);
}

float complex_get_angle(Complex a)
{
    return ((float)atan2(a.x, -a.y) * 180 / M_PI);
}

float complex_get_radius(Complex a)
{
    return ((float)sqrt(pow(a.x, 2) + pow(a.y, 2)));
}

float complex_get_distance_diff(Complex a, Complex b)
{
    return ((float)sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
}

Complex complex_stage(Complex a, float stage)
{
    float cur = complex_get_radius(a);

    if (cur > stage) {
        return (a / cur);
    }
    return (a);
}

Complex complex_normalize(Complex a)
{
    float rad = complex_get_radius(a);

    if (rad) {
        a.x = a.x / rad;
        a.y = a.y / rad;
    }
    return (a);
}
