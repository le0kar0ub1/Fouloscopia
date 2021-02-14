#ifndef __BOID_HPP__
# define __BOID_HPP__

# include "fouloscopia.hpp"
# include "Complex.hpp"

# define BOID_BY_GROUP 300 // number of boid by group

struct Color
{
    int r;
    int g;
    int b;
    int a;
    Color(int r, int g, int b, int a): r(r), g(g), b(b), a(a) {}
    Color() {}
};

enum BOID_HEALTH_STATE
{
    CLEAN = 0,
    INFECTED = 1,
    DEAD = 2,
    IMMUNE = 3
};

struct Health
{
    enum BOID_HEALTH_STATE state;
    int infected_clock;
};

struct Boid
{
    int deg;
    Color color;
    Complex pos;
    Complex velocity;
    Complex acc;
    Health health;
};

struct BoidGroup
{
    Boid boid[BOID_BY_GROUP];
    int nb;
};

void boid_init(BoidGroup &boids, int nb);
void boid_draw(BoidGroup &boids);
void boid_handle_world(BoidGroup &boids);
void boid_random_life(BoidGroup &boids);
void boid_update(BoidGroup &boids);

#endif /* __BOID_HPP__ */
