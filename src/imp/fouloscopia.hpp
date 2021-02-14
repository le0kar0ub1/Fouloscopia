#ifndef __FOULOSCOPIA_HPP__
# define __FOULOSCOPIA_HPP__

# include "Complex.hpp"
# include "Grapic.h"

# define MAX_X 500 // window size / 2 (this is a complex grid, you know)
# define MAX_Y 500 // window size / 2 (this is a complex grid, you know)

using namespace grapic;

struct Quantity // value which user can modulate
{
    float val; // the value
    float mod; // increase/decrease by mod step
};

extern Quantity boid_size;
extern Quantity repulsion_field;
extern Quantity alignment_field;
extern Quantity cohesion_field;
extern Quantity acceleration_weight;
extern Quantity velocity_weight;
extern Quantity physical_weight;
extern Quantity repulsion_weight;
extern Quantity alignment_weight;
extern Quantity cohesion_weight;
extern Quantity propagation_probability;
extern Quantity radius_propagation;
extern Quantity immunity_weight;
extern Quantity deathrate;
extern Quantity infection_duration;
extern Quantity boid_clean;
extern Quantity boid_infected;
extern Quantity boid_immune;
extern Quantity boid_dead;

extern bool grouping;
extern int  world;
extern bool updating;
extern bool randoming;

#endif /* __FOULOSCOPIA_HPP__ */