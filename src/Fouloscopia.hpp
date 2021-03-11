#ifndef __FOULOSCOPIA_HPP__
# define __FOULOSCOPIA_HPP__

# include <array>
# include "Boid.hpp"
# include "Grapic.h"

using namespace grapic;

# define MAX_X 500 // window size / 2 (this is a complex grid, you know)
# define MAX_Y 500 // window size / 2 (this is a complex grid, you know)

# define HEALTH_STEP 2

# define BOID_NUMBER 200 // number of boid by group at start
# define BOID_MAX_NUMBER 400 // max boid (can drop it using mouse)

# define VELOCITY_STEP 0.015

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
    Quantity(float v, float m) {val = v; mod = m;}
};

/**
 * The main class which will contain the public quantities needed by the boid to be updated
 */
class Fouloscopia
{
public:
    Fouloscopia();
    void run();
    ~Fouloscopia();

    std::vector<Boid *> *boids; // Our dynamic population of Boids

    bool grouping = true; // if true, the boids try to make group
    int world = WORLD_INFINITE;
    bool updating = true; // if true, pause the update
    bool randoming = false; // if true, put random in brid move

    Quantity boid_size = Quantity(10, 1); // size of one boid, can't move it in runtime, not because it's impossible, but useless
    Quantity repulsion_field = Quantity(20, 1); // field of action of the repulsion behavior
    Quantity alignment_field = Quantity(50, 1); // field of action of the alignment behavior
    Quantity cohesion_field = Quantity(20, 1); // field of action of the cohesion behavior

    Quantity acceleration_weight = Quantity(8.0, 0.2);
    Quantity velocity_weight = Quantity(BOID_NUMBER * VELOCITY_STEP, 0.2);
    Quantity physical_weight = Quantity(1.0, 0.2);

    Quantity repulsion_weight = Quantity(10.0, 0.5);
    Quantity alignment_weight = Quantity(1, 0.5);
    Quantity cohesion_weight = Quantity(0.2, 0.5);

    Quantity propagation_probability = Quantity(2.5, 0.2);
    Quantity radius_propagation = Quantity(30, 1);
    Quantity immunity_weight = Quantity(0.90, 0.02);
    Quantity deathrate = Quantity(0.4, 0.02);
    Quantity infection_duration = Quantity(10, 2 );

    Quantity boid_clean = Quantity(BOID_NUMBER, 0);
    Quantity boid_infected = Quantity(0, 0);
    Quantity boid_immune = Quantity(0, 0);
    Quantity boid_dead = Quantity(0, 0);

private:
    Quantity *focus = &propagation_probability;

    void init();
    void dynamic_health_information();
    void dynamic_simulation_information();
    void dynamic_information_focus(int x, int y, Quantity *cur);
    void handle_input();
    void remove_dead();
};

extern Fouloscopia simulation;

#endif /* __FOULOSCOPIA_HPP__ */
