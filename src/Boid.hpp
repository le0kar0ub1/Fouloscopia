#ifndef __BOID_HPP__
# define __BOID_HPP__

# include "Complex.hpp"

struct Color
{
    int r;
    int g;
    int b;
    int a;
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
    void Draw() const;
    void handle_world();
    void random_life();
    void update_pos();
    void update_health();
    ~Boid();
private:
    void handle_world_randomback();
    void handle_world_geometric();
    void handle_world_infinite();
    void repulsion();
    void alignement();
    void cohesion();
    double _deg;
    Color _color;
    Complex _pos;
    Complex _velocity;
    Complex _acc;
    Health _health;
};

#endif /* __BOID_HPP__ */
