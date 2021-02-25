#ifndef __BOID_HPP__
# define __BOID_HPP__

# include "Complex.hpp"

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

class Boid
{
public:
    Boid();
    Boid(const Boid &p2);
    void operator=(Boid const &a);
    void draw() const;
    void handle_world();
    void random_life();
    void set_pos(float x, float y);
    Complex pos(void) const;
    enum BOID_HEALTH_STATE health(void) const;
    void update_position();
    void update_health();
    void set_health(enum BOID_HEALTH_STATE state);
    void set_color(Color color);
    ~Boid();
private:
    void handle_world_randomback(void);
    void handle_world_geometric(void);
    void handle_world_infinite(void);
    Complex repulsion(void);
    Complex alignment(void);
    Complex cohesion(void);
    int _deg;
    Color _color;
    Complex _pos;
    Complex _velocity;
    Complex _acc;
    Health _health;
};

#endif /* __BOID_HPP__ */
