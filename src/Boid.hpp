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

enum BIRD_HEALTH_STATE
{
    CLEAN = 0,
    INFECTED = 1,
    DEAD = 2,
    IMMUNE = 3
};

struct Health
{
    enum BIRD_HEALTH_STATE state;
    int infected_clock;
};

class Boid
{
public:
    Boid();
    void draw() const;
    void handle_world();
    void random_life();
    void set_pos(float x, float y);
    void update_pos();
    void update_health();
    void set_health(enum BIRD_HEALTH_STATE state);
    void set_color(Color color);
    ~Boid();
private:
    void handle_world_randomback(void);
    void handle_world_geometric(void);
    void handle_world_infinite(void);
    Complex repulsion(void);
    Complex alignement(void);
    Complex cohesion(void);
    int _deg;
    Color _color;
    Complex _pos;
    Complex _velocity;
    Complex _acc;
    Health _health;
};

#endif /* __BOID_HPP__ */
