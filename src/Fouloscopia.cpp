# include "Fouloscopia.hpp"

Fouloscopia::Fouloscopia()
{
    _number = 0;

    while (_number < 300) {
        _boids.push_back(Boid());
        _number++;
    }
}

Fouloscopia::~Fouloscopia()
{
    _boids.clear();
}

Fouloscopia::run()
{

}