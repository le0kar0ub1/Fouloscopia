# include "Boid.hpp"
# include "Complex.hpp"

/**
 *  Initialize a boid group
 */
void boid_init(BoidGroup &boids, int nb)
{
    Color color = Color(255, 255, 255, 255);

    for (int i = 0; i < nb; i++) {
        boids.boid[i].pos = Complex(Point(0, 0));
        boids.boid[i].deg = rand() % 360;
        boids.boid[i].color = color;
        boids.boid[i].velocity = Complex(Point(rand() % 3 - 1, rand() % 3 - 1));
        boids.boid[i].acc = Complex(Point(0, 0));
        boids.boid[i].health.state = CLEAN;
        boids.boid[i].health.infected_clock = 0;
    }
    boids.nb = nb;
}

/**
 * Draw a whole boid group
 */
void boid_draw(BoidGroup &boids)
{
    for (int i = 0; i < boids.nb; i++) {
        if (boids.boid[i].health.state == DEAD)
            continue;
        int deg;
        if (grouping) {
            deg = ((int)boids.boid[i].velocity.get_angle() + 90) % 360;
        } else {
            deg = boids.boid[i].deg + 180;
        }
        Complex p2 = Complex(boid_size.val, (deg - (int)boid_size.val * 2) % 360);
        p2 = p2 + boids.boid[i].pos;
        Complex p3 = Complex(boid_size.val, (deg + (int)boid_size.val * 2) % 360);
        p3 = p3 + boids.boid[i].pos;
        color(boids.boid[i].color.r, boids.boid[i].color.g, boids.boid[i].color.b, boids.boid[i].color.a);
        triangleFill(
            (int)boids.boid[i].pos.x() + MAX_X, (int)boids.boid[i].pos.y() + MAX_Y,
            (int)p2.x() + MAX_X, (int)p2.y() + MAX_Y,
            (int)p3.x() + MAX_X, (int)p3.y() + MAX_Y
        );
    }
}

/**
 * If boid touch the wall then make him turn over 180° randomly +/- 45°
 */
static void boid_handle_world_randomback(Boid &boid)
{
    int randomed = (rand() % 90) - 45;
    if (boid.pos.x() < (-MAX_X + boid_size.val)) {
        if ((boid.deg >= 90 && boid.deg <= 180) || (boid.deg >= 180 && boid.deg <= 270)) {
            boid.deg = ((boid.deg + 180 + randomed) % 360);
        }
    } else if (boid.pos.y() > (MAX_Y - boid_size.val))  {
        if ((boid.deg >= 90 && boid.deg <= 180) || (boid.deg > 0 && boid.deg < 90)) {
            boid.deg = ((boid.deg + 180 + randomed) % 360);
        }
    } else if (boid.pos.y() < (-MAX_Y + boid_size.val)) {
        if ((boid.deg >= 180 && boid.deg <= 270) || (boid.deg >= 270 && boid.deg <= 360)) {
            boid.deg = ((boid.deg + 180 + randomed) % 360);
        }
    } else if (boid.pos.x() > (MAX_X - boid_size.val)) {
        if ((boid.deg >= 0 && boid.deg <= 90) || (boid.deg >= 270 && boid.deg <= 360)) {
            boid.deg = ((boid.deg + 180 + randomed) % 360);
        }
    }
}

/**
 * If boid touch the wall then make him turn over 90° in natural direction
 */
static void boid_handle_world_geometric(Boid &boid)
{
    if (boid.pos.x() < (-MAX_X + boid_size.val)) {
        if (boid.deg >= 90 && boid.deg <= 180) {
            boid.deg = (boid.deg - 90) % 360;
        } else if (boid.deg >= 180 && boid.deg <= 270) {
            boid.deg = (boid.deg + 90) % 360;
        }
    } else if (boid.pos.y() > (MAX_Y - boid_size.val))  {
        if (boid.deg >= 90 && boid.deg <= 180) {
            boid.deg = (boid.deg + 90) % 360;
        } else if (boid.deg > 0 && boid.deg < 90) {
            boid.deg = 360 + (boid.deg - 90);
        }
    } else if (boid.pos.y() < (-MAX_Y + boid_size.val)) {
        if (boid.deg >= 180 && boid.deg <= 270) {
            boid.deg = (boid.deg - 90) % 360;
        } else if (boid.deg >= 270 && boid.deg <= 360) {
            boid.deg = (boid.deg + 90) % 360;
        }
    } else if (boid.pos.x() > (MAX_X - boid_size.val)) {
        if (boid.deg >= 0 && boid.deg <= 90) {
            boid.deg = (boid.deg + 90) % 360;
        } else if (boid.deg >= 270 && boid.deg <= 360) {
            boid.deg = (boid.deg - 90) % 360;
        }
    }
}

/**
 * A world with a end but no border, like the pacman one
 */
static void boid_handle_world_infinite(Boid &boid)
{
    if (boid.pos.x() < -MAX_X)
        boid.pos.set_x(boid.pos.x() + MAX_X * 2);
    if (boid.pos.x() > MAX_X)
        boid.pos.set_x(boid.pos.x() - MAX_X * 2);
    if (boid.pos.y() < -MAX_Y)
        boid.pos.set_y(boid.pos.y() + MAX_Y * 2);
    if (boid.pos.y() > MAX_Y)
        boid.pos.set_y(boid.pos.y() - MAX_Y * 2);
}

/**
 * Work against wall problems if collision w/ a wall, see above
 */
void boid_handle_world(BoidGroup &boids)
{
    for (int i = 0; i < boids.nb; i++) {
        if (world == 0) {
            boid_handle_world_infinite(boids.boid[i]);
        } else if (world == 1) {
            boid_handle_world_geometric(boids.boid[i]);
        } else if (world == 2) {
            boid_handle_world_randomback(boids.boid[i]);
        }
    }
}

/**
 * Randomly move the boid orientation +/- 10°
 */
void boid_random_life(BoidGroup &boids)
{
    for (int i = 0; i < boids.nb; i++) {
        int randomed = rand() % 180;
        if (randomed <= 20) {
            boids.boid[i].deg = (boids.boid[i].deg + (randomed - 10)) % 360;
            if (boids.boid[i].deg < 0) {
                boids.boid[i].deg = 360 + boids.boid[i].deg;
            }
        }
    }
}

/**
 * Handle repulsion rule behavior
 */
static Complex boid_repulsion(BoidGroup &boids, int eval)
{
    Complex repulsion = Complex(Point(0, 0));
    int repulsion_count;

    for (int i = 0; i < boids.nb; i++) {
        float dist = boids.boid[i].pos.get_distance(boids.boid[eval].pos);
        if (dist > 0 && dist < repulsion_field.val) {
            repulsion = repulsion + ((boids.boid[eval].pos - boids.boid[i].pos).normalize() / dist);
            repulsion_count++;
        }
    }
    if (repulsion_count > 0) {
        repulsion = repulsion / (float)repulsion_count;
    }
    if (repulsion.get_radius() > 0) {
        repulsion = (repulsion.normalize() * velocity_weight.val) - boids.boid[eval].velocity;
        repulsion = repulsion.stage(physical_weight.val);
    }
    return (repulsion);
}

/**
 * Handle alignment rule behavior
 */
static Complex boid_alignment(BoidGroup &boids, int eval)
{
    Complex alignment = Complex(Point(0, 0));
    int alignment_count;

    for (int i = 0; i < boids.nb; i++) {
        float dist = boids.boid[i].pos.get_distance(boids.boid[eval].pos);
        if (dist > 0 && dist < alignment_field.val) {
            alignment = alignment + boids.boid[i].velocity;
            alignment_count++;
        }
    }
    if (alignment_count > 0) {
        alignment = (alignment / (float)alignment_count).normalize() * velocity_weight.val;
        alignment = (alignment - boids.boid[eval].velocity).stage(physical_weight.val);
    }
    return (alignment);
}

/**
 * Handle cohesion rule behavior
 */
static Complex boid_cohesion(BoidGroup &boids, int eval)
{
    Complex cohesion = Complex(Point(0, 0));
    int cohesion_count;

    for (int i = 0; i < boids.nb; i++) {;
        float dist = boids.boid[i].pos.get_distance(boids.boid[eval].pos);
        if (dist > 0 && dist < cohesion_field.val) {;
            cohesion = cohesion + boids.boid[i].pos;
            cohesion_count++;
        }
    }
    if (cohesion_count > 0) {
        cohesion = cohesion / (float)cohesion_count;
        cohesion = Complex(0, 0) - cohesion;
        cohesion = (cohesion.normalize() * velocity_weight.val);
        boids.boid[eval].acc = cohesion - boids.boid[eval].velocity;
        boids.boid[eval].acc = boids.boid[eval].acc.stage(physical_weight.val);
    }
    return (cohesion);
}

/**
 * The core of the simulation purpose
 * All of the update isn't well defined, in fact it's a heap of scalar which we can modulate to obtain more or less a group
 * I (actually) don't think that the current parameters are the best one
 */
void boid_update(BoidGroup &boids)
{
    for (int i = 0; i < boids.nb; i++) {
        Complex repulsion = boid_repulsion(boids, i); // compute the repulsion
        Complex alignment = boid_alignment(boids, i); // compute the alignment
        Complex cohesion = boid_cohesion(boids, i); // compute the cohesion

        repulsion = repulsion * repulsion_weight.val; // weight the repulsion by the given qunatity
        alignment = alignment * alignment_weight.val; // weight the alignment by the given qunatity
        cohesion  = cohesion * cohesion_weight.val;  // weight the cohesion by the given qunatity

        boids.boid[i].acc = boids.boid[i].acc + repulsion; // add the repulsion vector to the acceleration
        boids.boid[i].acc = boids.boid[i].acc + alignment; // add the alignment vector to the acceleration
        boids.boid[i].acc = boids.boid[i].acc + cohesion; // add the cohesion vector to the acceleration
        
        boids.boid[i].acc = boids.boid[i].acc * acceleration_weight.val; // weight the acceleration
        if (grouping) {
            boids.boid[i].velocity = (boids.boid[i].velocity + boids.boid[i].acc).stage(velocity_weight.val);
            boids.boid[i].pos = boids.boid[i].pos + boids.boid[i].velocity * velocity_weight.val;
        } else {
            boids.boid[i].velocity = Complex(velocity_weight.val, boids.boid[i].deg);
            boids.boid[i].pos = boids.boid[i].pos + boids.boid[i].velocity;
        }
        boids.boid[i].acc = boids.boid[i].acc * 0; // reset acc
    }
}

/**
 * Built-in random to fight against low probability
 */
static bool propagation_random(float max, float proba)
{
    return ((rand() % (int)max) < (int)(proba * max));
}

/**
 *  Update boid health
 */
void update_health(BoidGroup &boids)
{
    for (int i = 0; i < boids.nb; i++) {
        // IMMUNE: make it simple and keep immunity forever, in fact it's obv not the case
        // CLEAN: clean boid make nothing
        // DEAD: dead boid can't revive, hard truth :/
        // INFECTED: infect others and die if no chance
        // All the probability are weighted by the disease time and update call to get as close as possible to the usual usage of r0 etc...
        if (boids.boid[i].health.state == INFECTED) {
            if (propagation_random(1000, deathrate.val / (infection_duration.val * HEALTH_STEP))) { // die if the life isn't nice
                boids.boid[i].health.state = DEAD;
                boid_infected.val -= 1;
                boid_dead.val += 1;
            } else if (boids.boid[i].health.infected_clock++ == (infection_duration.val * HEALTH_STEP)) { // cure if the life is nice and become IMMUNE or CLEAN depend of the immunity weight
                boids.boid[i].health.infected_clock = 0;
                boid_infected.val -= 1;
                if (propagation_random(1000, immunity_weight.val)) {
                    boid_immune.val += 1;
                    boids.boid[i].health.state = IMMUNE;
                    boids.boid[i].color = Color(0, 255, 0, 255);
                } else {
                    boid_clean.val += 1;
                    boids.boid[i].health.state = CLEAN;
                    boids.boid[i].color = Color(255, 255, 255, 255);
                }
            } else { // infect other friends
                for (int j = 0; j < boids.nb; j++) {
                    if (boids.boid[j].health.state == CLEAN && j != i) {
                        float dist = boids.boid[i].pos.get_distance(boids.boid[j].pos);
                        if (dist < radius_propagation.val && propagation_random(1000, propagation_probability.val / (infection_duration.val * HEALTH_STEP))) {
                            boid_clean.val -= 1;
                            boid_infected.val += 1;
                            boids.boid[j].health.state = INFECTED;
                            boids.boid[j].color = Color(255, 0, 0, 255);
                        }
                    }
                }
            }   
        }
    }
}