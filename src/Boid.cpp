# include "Boid.hpp"
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
    _pos = Complex(Point(0, 0));
    _deg = rand() % 360;
    _color = color_init(255, 255, 255, 255);
    _velocity = Complex(Point((double)(rand() % 3 - 2), (double)(rand() % 3 - 2)));
    _acc = Complex(Point(0., 0.));
    _health.state = CLEAN;
    _health.infected_clock = 0;
}

Boid::~Boid() {}

void Boid::Draw() const
{
    int deg;

    if (_health.state == DEAD)
        return;
    if (grouping) {
        deg = ((int)complex_get_angle(birds.bird[i].velocity) + 90) % 360;
    } else {
        deg = birds.bird[i].deg + 180;
    }
    Complex p2 = complex_init_polar(bird_size.val, (deg - (int)bird_size.val * 2) % 360);
    p2 = p2 + birds.bird[i].pos;
    Complex p3 = complex_init_polar(bird_size.val, (deg + (int)bird_size.val * 2) % 360);
    p3 = p3 + birds.bird[i].pos;
    color(birds.bird[i].color.r, birds.bird[i].color.g, birds.bird[i].color.b, birds.bird[i].color.a);
    triangleFill(
        (int)birds.bird[i].pos.x + MAX_X, (int)birds.bird[i].pos.y + MAX_Y,
        (int)p2.x + MAX_X, (int)p2.y + MAX_Y,
        (int)p3.x + MAX_X, (int)p3.y + MAX_Y
    );
}

void Boid::handle_world() {}
void Boid::random_life() {}
void Boid::update_pos() {}
void Boid::update_health() {}
void Boid::handle_world_randomback() {}
void Boid::handle_world_geometric() {}
void Boid::handle_world_infinite() {}
void Boid::repulsion() {}
void Boid::alignement() {}
void Boid::cohesion() {}