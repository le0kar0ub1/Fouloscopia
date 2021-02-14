# include "Boid.hpp"
# include "Fouloscopia.hpp"
# include "Grapic.h"

Boid::Boid()
{
    _pos = Complex(Point(0, 0));
    _deg = rand() % 360;
    _color = Color(255, 255, 255, 255);
    _velocity = Complex(Point(rand() % 3 - 1, rand() % 3 - 1));
    _acc = Complex(Point(0, 0));
    _health.state = CLEAN;
    _health.infected_clock = 0;
}

Boid::~Boid() {}

Complex Boid::pos(void) const
{
    return (_pos);
}

void Boid::set_pos(float x, float y)
{
    _pos.set(x, y);
}

void Boid::set_health(enum BOID_HEALTH_STATE state)
{
    _health.state = state;
}

void Boid::set_color(Color color)
{
    _color = color;
}

void Boid::draw() const
{
    int deg;

    if (_health.state == DEAD)
        return;
    if (simulation.grouping) {
        deg = ((int)_velocity.get_angle() + 90) % 360;
    } else {
        deg = (_deg + 180) % 360;
    }
    Complex p2(simulation.boid_size.val, (deg - (int)simulation.boid_size.val * 2) % 360);
    p2 = p2 + _pos;
    Complex p3(simulation.boid_size.val, (deg + (int)simulation.boid_size.val * 2) % 360);
    p3 = p3 + _pos;
    color(_color.r, _color.g, _color.b, _color.a);
    triangleFill(
        (int)_pos.x() + MAX_X, (int)_pos.y() + MAX_Y,
        (int)p2.x() + MAX_X, (int)p2.y() + MAX_Y,
        (int)p3.x() + MAX_X, (int)p3.y() + MAX_Y
    );
}

/**
 * If boid touch the wall then make him turn over 180° randomly +/- 45°
 */
void Boid::handle_world_randomback(void)
{
    int randomed = (rand() % 90) - 45;
    if (_pos.x() < (-MAX_X + simulation.boid_size.val)) {
        if ((_deg >= 90 && _deg <= 180) || (_deg >= 180 && _deg <= 270)) {
            _deg = ((_deg + 180 + randomed) % 360);
        }
    } else if (_pos.y() > (MAX_Y - simulation.boid_size.val))  {
        if ((_deg >= 90 && _deg <= 180) || (_deg > 0 && _deg < 90)) {
            _deg = ((_deg + 180 + randomed) % 360);
        }
    } else if (_pos.y() < (-MAX_Y + simulation.boid_size.val)) {
        if ((_deg >= 180 && _deg <= 270) || (_deg >= 270 && _deg <= 360)) {
            _deg = ((_deg + 180 + randomed) % 360);
        }
    } else if (_pos.x() > (MAX_X - simulation.boid_size.val)) {
        if ((_deg >= 0 && _deg <= 90) || (_deg >= 270 && _deg <= 360)) {
            _deg = ((_deg + 180 + randomed) % 360);
        }
    }
}

/**
 * If boid touch the wall then make him turn over 90° in natural direction
 */
void Boid::handle_world_geometric(void)
{
    if (_pos.x() < (-MAX_X + simulation.boid_size.val)) {
        if (_deg >= 90 && _deg <= 180) {
            _deg = (_deg - 90) % 360;
        } else if (_deg >= 180 && _deg <= 270) {
            _deg = (_deg + 90) % 360;
        }
    } else if (_pos.y() > (MAX_Y - simulation.boid_size.val))  {
        if (_deg >= 90 && _deg <= 180) {
            _deg = (_deg + 90) % 360;
        } else if (_deg > 0 && _deg < 90) {
            _deg = 360 + (_deg - 90);
        }
    } else if (_pos.y() < (-MAX_Y + simulation.boid_size.val)) {
        if (_deg >= 180 && _deg <= 270) {
            _deg = (_deg - 90) % 360;
        } else if (_deg >= 270 && _deg <= 360) {
            _deg = (_deg + 90) % 360;
        }
    } else if (_pos.x() > (MAX_X - simulation.boid_size.val)) {
        if (_deg >= 0 && _deg <= 90) {
            _deg = (_deg + 90) % 360;
        } else if (_deg >= 270 && _deg <= 360) {
            _deg = (_deg - 90) % 360;
        }
    }
}

/**
 * A world with a end but no border, like the pacman one
 */
void Boid::handle_world_infinite(void)
{
    if (_pos.x() < -MAX_X)
        _pos.set_x(_pos.x() + MAX_X * 2);
    else if (_pos.x() > MAX_X)
        _pos.set_x(_pos.x() - MAX_X * 2);
    if (_pos.y() < -MAX_Y)
        _pos.set_y(_pos.y() + MAX_Y * 2);
    else if (_pos.y() > MAX_Y)
        _pos.set_y(_pos.y() - MAX_Y * 2);
}

/**
 * Work against wall problems if collision w/ a wall, see above
 */
void Boid::handle_world(void)
{
    if (simulation.world == WORLD_INFINITE) {
        handle_world_infinite();
    } else if (simulation.world == WORLD_GEOMETRIC) {
        handle_world_geometric();
    } else if (simulation.world == WORLD_RANDOMBACK) {
        handle_world_randomback();
    }
}

/**
 * Randomly move the boid orientation +/- 10°
 */
void Boid::random_life(void)
{
    int randomed = rand() % 180;

    if (randomed <= 20) {
        _deg = (_deg + (randomed - 10)) % 360;
        if (_deg < 0) {
            _deg = 360 + _deg;
        }
    }
}

/**
 * Handle repulsion rule behavior
 */
Complex Boid::repulsion(void)
{
    Complex repulsion(Point(0, 0));
    int repulsion_count;

    for (int i = 0; i != BOID_BY_GROUP; i++) {
        float dist = _pos.get_distance(simulation.boids[i]._pos);
        if (dist > 0 && dist < simulation.repulsion_field.val) {
            repulsion = repulsion + ((_pos - simulation.boids[i]._pos).normalize() / dist);
            repulsion_count++;
        }
    }
    if (repulsion_count > 0) {
        repulsion = repulsion / (float)repulsion_count;
    }
    if (repulsion.get_radius() > 0) {
        repulsion = (repulsion.normalize() * simulation.velocity_weight.val) - _velocity;
        repulsion = repulsion.stage(simulation.physical_weight.val);
    }
    return (repulsion);
}

/**
 * Handle alignment rule behavior
 */
Complex Boid::alignment(void)
{
    Complex alignment(Point(0, 0));
    int alignment_count;

    for (int i = 0; i != BOID_BY_GROUP; i++) {
        float dist = _pos.get_distance(simulation.boids[i]._pos);
        if (dist > 0 && dist < simulation.alignment_field.val) {
            alignment = alignment + simulation.boids[i]._velocity;
            alignment_count++;
        }
    }
    if (alignment_count > 0) {
        alignment = (alignment / (float)alignment_count).normalize() * simulation.velocity_weight.val;
        alignment = (alignment - _velocity).stage(simulation.physical_weight.val);
    }
    return (alignment);
}

/**
 * Handle cohesion rule behavior
 */
Complex Boid::cohesion(void)
{
    Complex cohesion(Point(0, 0));
    int cohesion_count;

    for (int i = 0; i != BOID_BY_GROUP; i++){;
        float dist = _pos.get_distance(simulation.boids[i]._pos);
        if (dist > 0 && dist < simulation.cohesion_field.val) {;
            cohesion = cohesion + simulation.boids[i]._pos;
            cohesion_count++;
        }
    }
    if (cohesion_count > 0) {
        cohesion = cohesion / (float)cohesion_count;
        cohesion = Complex(0, 0) - cohesion;
        cohesion = cohesion.normalize() * simulation.velocity_weight.val;
        _acc = (cohesion - _velocity).stage(simulation.physical_weight.val);
    }
    return (cohesion);
}

/**
 * The core of the simulation purpose
 * All of the update isn't well defined, in fact it's a heap of scalar which we can modulate to obtain more or less a group
 * I (actually) don't think that the current parameters are the best one
 */
void Boid::update_position(void)
{
    Complex repulsion = this->repulsion(); // compute the repulsion
    Complex alignment = this->alignment(); // compute the alignment
    Complex cohesion = this->cohesion(); // compute the cohesion

    repulsion = repulsion * simulation.repulsion_weight.val; // weight the repulsion by the given qunatity
    alignment = alignment * simulation.alignment_weight.val; // weight the alignment by the given qunatity
    cohesion  = cohesion * simulation.cohesion_weight.val;  // weight the cohesion by the given qunatity

    _acc = _acc + repulsion; // add the repulsion vector to the acceleration
    _acc = _acc + alignment; // add the alignment vector to the acceleration
    _acc = _acc + cohesion; // add the cohesion vector to the acceleration
    
    _acc = _acc * simulation.acceleration_weight.val; // weight the acceleration

    if (simulation.grouping) {
        _velocity = (_velocity + _acc).stage(simulation.velocity_weight.val);
        _pos = _pos + _velocity * simulation.velocity_weight.val;
    } else {
        _velocity = Complex(simulation.velocity_weight.val, (float)_deg);
        _pos = _pos + _velocity;
    }
    _acc = _acc * 0; // reset acc
}

/**
 * Built-in random to fight against low probability
 */
static bool propagation_random(int max, float proba)
{
    return ((rand() % max) < (int)(proba * max));
}

void Boid::update_health()
{
    // IMMUNE: make it simple and keep immunity forever, in fact it's obv not the case
    // CLEAN: clean boid make nothing
    // DEAD: dead boid can't revive, hard truth :/
    // INFECTED: infect others, become immune or come back to clean, and die if no chance
    // All the probabilities are weighted by the disease time and update call to get as close as possible to the usual usage of r0 etc...
    if (_health.state == INFECTED) {
        if (propagation_random(1000, simulation.deathrate.val / (simulation.infection_duration.val * HEALTH_STEP))) { // die if the life isn't nice
            _health.state = DEAD;
            simulation.boid_infected.val -= 1;
            simulation.boid_dead.val += 1;
        } else if (_health.infected_clock++ == (simulation.infection_duration.val * HEALTH_STEP)) { // cure if the life is nice and become IMMUNE or CLEAN depend of the immunity weight
            _health.infected_clock = 0;
            simulation.boid_infected.val -= 1;
            if (propagation_random(1000, simulation.immunity_weight.val)) {
                simulation.boid_immune.val += 1;
                _health.state = IMMUNE;
                _color = Color(0, 255, 0, 255);
            } else {
                simulation.boid_clean.val += 1;
                _health.state = CLEAN;
                _color = Color(255, 255, 255, 255);
            }
        } else { // infect other friends
            for (int i = 0; i != BOID_BY_GROUP; i++) {
                if (simulation.boids[i]._health.state == CLEAN) {
                    float dist = _pos.get_distance(simulation.boids[i]._pos);
                    if (dist < simulation.radius_propagation.val && propagation_random(1000, simulation.propagation_probability.val / (simulation.infection_duration.val * HEALTH_STEP))) {
                        simulation.boid_clean.val -= 1;
                        simulation.boid_infected.val += 1;
                        simulation.boids[i]._health.state = INFECTED;
                        simulation.boids[i]._color = Color(255, 0, 0, 255);
                    }
                }
            }
        }
    }
}