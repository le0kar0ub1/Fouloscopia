#ifndef __COMPLEX_HPP__
# define __COMPLEX_HPP__

# include <math.h>
# include <iostream>

struct Complex
{
    float x;
    float y;
};

Complex operator+(Complex a, Complex b);
Complex operator-(Complex a, Complex b);
Complex operator*(Complex a, Complex b);
Complex operator*(Complex a, float scalar);
Complex operator/(Complex a, float scalar);

Complex complex_init_cartesian(float x, float y);
Complex complex_init_polar(float radius, float deg);
float complex_get_angle(Complex a);
float complex_get_radius(Complex a);
float complex_get_distance_diff(Complex a, Complex b);
Complex complex_stage(Complex a, float stage);
Complex complex_normalize(Complex a);

#endif /* __COMPLEX_HPP__ */