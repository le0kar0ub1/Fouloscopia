# include "Boid.hpp"
# include "Fouloscopia.hpp"
# include "Grapic.h"

Color color_init(int r, int g, int b, int a)
{
    Color color = {
        .r = r,
        .g = g,
        .b = b,
        .a = a
    };
    return (color);
}

Boid::Boid()
{
    _pos = Complex(Point(0,0)); //rand() % 1000 - 500, rand() % 1000 - 500));
    _deg = rand() % 360;
    _color = color_init(255, 255, 255, 255);
    _velocity = Complex(Point((rand() % 3 - 2), (rand() % 3 - 2)));
    _acc = Complex(Point(0, 0));
    _health.state = CLEAN;
    _health.infected_clock = 0;
}

Boid::~Boid() {}

void Boid::set_pos(float x, float y)
{
    _pos.set(x, y);
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
    Complex p2(simulation.bird_size.val, (deg - (int)simulation.bird_size.val * 2) % 360);
    p2 = p2 + _pos;
    Complex p3(simulation.bird_size.val, (deg + (int)simulation.bird_size.val * 2) % 360);
    p3 = p3 + _pos;
    color(_color.r, _color.g, _color.b, _color.a);
    triangleFill(
        (int)_pos.x() + MAX_X, (int)_pos.y() + MAX_Y,
        (int)p2.x() + MAX_X, (int)p2.y() + MAX_Y,
        (int)p3.x() + MAX_X, (int)p3.y() + MAX_Y
    );
}

/**
 * If bird touch the wall then make him turn over 180° randomly +/- 45°
 */
void Boid::handle_world_randomback(void)
{
    int randomed = (rand() % 90) - 45;
    if (_pos.x() < (-MAX_X + simulation.bird_size.val)) {
        if ((_deg >= 90 && _deg <= 180) || (_deg >= 180 && _deg <= 270)) {
            _deg = ((_deg + 180 + randomed) % 360);
        }
    } else if (_pos.y() > (MAX_Y - simulation.bird_size.val))  {
        if ((_deg >= 90 && _deg <= 180) || (_deg > 0 && _deg < 90)) {
            _deg = ((_deg + 180 + randomed) % 360);
        }
    } else if (_pos.y() < (-MAX_Y + simulation.bird_size.val)) {
        if ((_deg >= 180 && _deg <= 270) || (_deg >= 270 && _deg <= 360)) {
            _deg = ((_deg + 180 + randomed) % 360);
        }
    } else if (_pos.x() > (MAX_X - simulation.bird_size.val)) {
        if ((_deg >= 0 && _deg <= 90) || (_deg >= 270 && _deg <= 360)) {
            _deg = ((_deg + 180 + randomed) % 360);
        }
    }
}

/**
 * If bird touch the wall then make him turn over 90° in natural direction
 */
void Boid::handle_world_geometric(void)
{
    if (_pos.x() < (-MAX_X + simulation.bird_size.val)) {
        if (_deg >= 90 && _deg <= 180) {
            _deg = (_deg - 90) % 360;
        } else if (_deg >= 180 && _deg <= 270) {
            _deg = (_deg + 90) % 360;
        }
    } else if (_pos.y() > (MAX_Y - simulation.bird_size.val))  {
        if (_deg >= 90 && _deg <= 180) {
            _deg = (_deg + 90) % 360;
        } else if (_deg > 0 && _deg < 90) {
            _deg = 360 + (_deg - 90);
        }
    } else if (_pos.y() < (-MAX_Y + simulation.bird_size.val)) {
        if (_deg >= 180 && _deg <= 270) {
            _deg = (_deg - 90) % 360;
        } else if (_deg >= 270 && _deg <= 360) {
            _deg = (_deg + 90) % 360;
        }
    } else if (_pos.x() > (MAX_X - simulation.bird_size.val)) {
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
    if (_pos.x() > MAX_X)
        _pos.set_x(_pos.x() - MAX_X * 2);
    if (_pos.y() < -MAX_Y)
        _pos.set_y(_pos.y() + MAX_Y * 2);
    if (_pos.y() > MAX_Y)
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
 * Randomly move the bird orientation +/- 10°
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

    for (std::list<Boid>::iterator boid = simulation.boids.begin(); boid != simulation.boids.end(); boid++) {
        float dist = _pos.get_distance_diff(boid->_pos);
        if (dist > 0 && dist < simulation.repulsion_field.val) {
            repulsion = repulsion + ((_pos - boid->_pos).normalize() / dist);
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
 * Handle alignement rule behavior
 */
Complex Boid::alignement(void)
{
    Complex alignement(Point(0, 0));
    int alignement_count;

    for (std::list<Boid>::iterator boid = simulation.boids.begin(); boid != simulation.boids.end(); boid++) {
        float dist = _pos.get_distance_diff(boid->_pos);
        if (dist > 0 && dist < simulation.alignement_field.val) {
            alignement = alignement + boid->_velocity;
            alignement_count++;
        }
    }
    if (alignement_count > 0) {
        alignement = (alignement / (float)alignement_count).normalize() * simulation.velocity_weight.val;
        alignement = (alignement - _velocity).stage(simulation.physical_weight.val);
    }
    return (alignement);
}

/**
 * Handle cohesion rule behavior
 */
Complex Boid::cohesion(void)
{
    Complex cohesion(Point(0, 0));
    int cohesion_count;

    for (std::list<Boid>::iterator boid = simulation.boids.begin(); boid != simulation.boids.end(); boid++){;
        float dist = _pos.get_distance_diff(boid->_pos);
        if (dist > 0 && dist < simulation.cohesion_field.val) {;
            cohesion = cohesion + boid->_pos;
            cohesion_count++;
        }
    }
    if (cohesion_count > 0) {
        cohesion = cohesion / (float)cohesion_count;
        cohesion = Complex(0, 0) - cohesion;
        cohesion = cohesion.normalize() * simulation.velocity_weight.val;
        _acc = cohesion - _velocity;
        _acc = _acc.stage(simulation.physical_weight.val);
    }
    return (cohesion);
}

/**
 * The core of the simulation purpose
 * All of the update isn't well defined, in fact it's a heap of scalar which we can modulate to obtain more or less a group
 * I (actually) don't think that the current parameters are the best one
 */
void Boid::update_pos(void)
{
    Complex repulsion = this->repulsion(); // compute the repulsion
    Complex alignement = this->alignement(); // compute the alignement
    Complex cohesion = this->cohesion(); // compute the cohesion

    repulsion = repulsion * simulation.repulsion_weight.val; // weight the repulsion by the given qunatity
    alignement = alignement * simulation.alignement_weight.val; // weight the alignement by the given qunatity
    cohesion  = cohesion * simulation.cohesion_weight.val;  // weight the cohesion by the given qunatity

    _acc = _acc + repulsion; // add the repulsion vector to the acceleration
    _acc = _acc + alignement; // add the alignement vector to the acceleration
    _acc = _acc + cohesion; // add the cohesion vector to the acceleration
    
    _acc = _acc * simulation.acceleration_weight.val; // weight the acceleration
    if (simulation.grouping) {
        _velocity = _velocity + _acc;
        _velocity.stage(simulation.velocity_weight.val);
        _pos = _pos + _velocity * simulation.velocity_weight.val;
    } else {
        _velocity = Complex(simulation.velocity_weight.val, (float)_deg);
        _pos = _pos + _velocity;
    }
    _acc = _acc * 0; // reset acc
}

void Boid::update_health() {}