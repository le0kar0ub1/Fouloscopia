#ifndef __FOULOSCOPIA_HPP__
# define __FOULOSCOPIA_HPP__

# include <list>
# include "Boid.hpp"

# define MAX_X 500 // window size / 2 (this is a complex grid, you know)
# define MAX_Y 500 // window size / 2 (this is a complex grid, you know)

# define BIRD_BY_GROUP 300 // number of bird by group

enum WORLD_TYPE
{
    WORLD_INFINITE = 0,
    WORLD_GEOMETRIC = 1,
    WORLD_RANDOMBACK = 2
};

struct Quantity // value which user can modulate
{
    float val; // the value
    float mod; // increase/decrease by mod step
    Quantity(float val, float mod) {val = val; mod = mod;}
};

class Fouloscopia
{
public:
    Fouloscopia();
    void run();
    ~Fouloscopia();
public:
    std::list<Boid> _boids; 
    int _number;
    bool grouping = true; // if true, the birds try to group
    enum WORLD_TYPE world = WORLD_INFINITE;
    bool updating = true; // if true, pause the update
    bool randoming = true; // if true, put random in brid move
    Quantity bird_size = Quantity(10, 1); // size of one bird
    Quantity repulsion_field = Quantity(20, 1); // field of action of the behavior
    Quantity alignement_field = Quantity(50, 1); // field of action of the behavior
    Quantity cohesion_field = Quantity(20, 1); // field of action of the behavior
    Quantity acceleration_weight = Quantity(1, 0.2);
    Quantity velocity_weight = Quantity(6, 0.2);
    Quantity physical_weight = Quantity(1.0, 0.2);
    Quantity repulsion_weight = Quantity(10.0, 0.5);
    Quantity alignement_weight = Quantity(1.0, 0.5);
    Quantity cohesion_weight = Quantity(0.2, 0.5);
    Quantity propagation_probability = Quantity(2.5, 0.2);
    Quantity radius_propagation = Quantity(20, 1);
    Quantity immunity_weight = Quantity(0.90, 0.02);
    Quantity deathrate = Quantity(0.4, 0.02);
    Quantity infection_duration = Quantity(20, 2 );
    Quantity bird_clean = Quantity(BIRD_BY_GROUP - 1, 0);
    Quantity bird_infected = Quantity(1, 0);
    Quantity bird_immune = Quantity(0, 0);
    Quantity bird_dead = Quantity(0, 0);
};

#endif /* __FOULOSCOPIA_HPP__ */
