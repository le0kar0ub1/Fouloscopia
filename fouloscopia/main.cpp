# include <Grapic.h>
# include <iostream>
# include <string>

using namespace std;
using namespace grapic;

# define MAX_X 500 // window size / 2 (this is a complex grid, you know)
# define MAX_Y 500 // window size / 2 (this is a complex grid, you know)

# define BIRD_BY_GROUP 300 // number of bird by group

# define BIRD_MAX_SIZE 10 // size of one bird

bool grouping = false; // if true, the birds try to group
int  world = 0; // 0 == infinite | 1 == geometric | 2 == random back
bool updating = true; // if true, pause the update
bool randoming = true; // if true, put random in brid move

struct Modulate // quantity which user can modulate
{
    float quantity; // the quantity
    float mod; // increase/decrease by mods step
};

Modulate repulsion_field  = {.quantity = 20, .mod = 1};
Modulate alignement_field = {.quantity = 50, .mod = 1};
Modulate cohesion_field   = {.quantity = 20, .mod = 1};

Modulate acceleration_weight = {.quantity = 1, .mod = 0.2};
Modulate velocity_weight     = {.quantity = 3.5, .mod = 0.2};
Modulate physical_weight     = {.quantity = 1.0, .mod = 0.2};

Modulate repulsion_weight  = {.quantity = 10.0, .mod = 0.2};
Modulate alignement_weight = {.quantity = 1.0, .mod = 0.2};
Modulate cohesion_weight   = {.quantity = 0.2, .mod = 0.2};

Modulate propagation_probability = {.quantity = 0.40, .mod = 0.02};
Modulate radius_propagation = {.quantity = 5, .mod = 1};
Modulate immunity_weight = {.quantity = 0.90, .mod = 0.02};
Modulate deathrate = {.quantity = 0.02, .mod = 0.02};

Modulate bird_clean = {.quantity = BIRD_BY_GROUP - 1, .mod = 0};
Modulate bird_infected = {.quantity = 1, .mod = 0};
Modulate bird_immune = {.quantity = 0, .mod = 0};
Modulate bird_dead = {.quantity = 0, .mod = 0};

Modulate *input_focus = &propagation_probability;

struct Color
{
    int r;
    int g;
    int b;
    int a;
};

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

struct Complex
{
    float x;
    float y;
};

Complex operator+(Complex a, Complex b)
{
    a.x += b.x;
    a.y += b.y;
    return (a);
}

Complex operator-(Complex a, Complex b)
{
    a.x -= b.x;
    a.y -= b.y;
    return (a);
}

Complex operator*(Complex a, Complex b)
{
    Complex ret = {
        .x = a.x * b.x - a.y * b.y,
        .y = a.x * b.y + a.y * b.x
    };
    return (ret);
}

Complex operator*(Complex a, float scalar)
{
    a.x = a.x * scalar;
    a.y = a.y * scalar;
    return (a);
}

Complex operator/(Complex a, float scalar)
{
    Complex ret = {
        .x = a.x / scalar,
        .y = a.y / scalar
    };
    return (ret);
}

Complex complex_init_cartesian(float x, float y)
{
    Complex ini = {
        .x = x,
        .y = y
    };
    return (ini);
}

Complex complex_init_polar(float radius, float deg)
{
    Complex ini = {
        .x = radius * (float)cos(deg / 180.0 * M_PI),
        .y = radius * (float)sin(deg / 180.0 * M_PI)
    };
    return (ini);
}

float complex_get_angle(Complex a)
{
    return ((float)atan2(a.x, -a.y) * 180 / M_PI);
}

float complex_get_radius(Complex a)
{
    return ((float)sqrt(pow(a.x, 2) + pow(a.y, 2)));
}

float complex_get_distance_diff(Complex a, Complex b)
{
    return ((float)sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
}

Complex complex_stage(Complex a, float stage)
{
    float cur = complex_get_radius(a);

    if (cur > stage) {
        return (a / cur);
    }
    return (a);
}

Complex complex_normalize(Complex a)
{
    float rad = complex_get_radius(a);

    if (rad) {
        a.x = a.x / rad;
        a.y = a.y / rad;
    }
    return (a);
}

enum BIRD_HEALTH
{
    CLEAN = 0,
    INFECTED = 1,
    DEAD = 2,
    IMMUNE = 3
};

struct Health
{
    enum BIRD_HEALTH state;
    float immunity;
    int infected_clock;
};

struct Bird
{
    int deg;
    Color color;
    Complex pos;
    Complex velocity;
    Complex acc;
    Health health;
};

struct BirdGroup
{
    Bird bird[BIRD_BY_GROUP];
    int nb;
};

/**
 *  Initialize a bird group
 */
void bird_init(BirdGroup &birds, int nb)
{
    Color color = color_init(255, 255, 255, 255);

    for (int i = 0; i < nb; i++) {
        birds.bird[i].pos.x = 0;
        birds.bird[i].pos.y = 0;
        birds.bird[i].deg = rand() % 360;
        birds.bird[i].color = color;
        birds.bird[i].velocity = complex_init_cartesian(rand() % 3 - 2, rand() % 3 - 2);
        birds.bird[i].acc = complex_init_cartesian(0, 0);
    }
    birds.nb = nb;
}

/**
 * Draw a whole bird group
 */
void bird_draw(BirdGroup &birds)
{
    for (int i = 0; i < birds.nb; i++) {
        if (birds.bird[i].health.state == DEAD)
            continue;
        int deg;
        if (grouping) {
            deg = ((int)complex_get_angle(birds.bird[i].velocity) + 90) % 360;
        } else {
            deg = birds.bird[i].deg + 180;
        }
        Complex p2 = complex_init_polar(BIRD_MAX_SIZE, (deg - BIRD_MAX_SIZE * 2) % 360);
        p2 = p2 + birds.bird[i].pos;
        Complex p3 = complex_init_polar(BIRD_MAX_SIZE, (deg + BIRD_MAX_SIZE * 2) % 360);
        p3 = p3 + birds.bird[i].pos;
        color(birds.bird[i].color.r, birds.bird[i].color.g, birds.bird[i].color.b, birds.bird[i].color.a);
        triangleFill(
            (int)birds.bird[i].pos.x + MAX_X, (int)birds.bird[i].pos.y + MAX_Y,
            (int)p2.x + MAX_X, (int)p2.y + MAX_Y,
            (int)p3.x + MAX_X, (int)p3.y + MAX_Y
        );
    }
}

/**
 * If bird touch the wall then make him turn over 180° randomly +/- 45°
 */
void bird_handle_world_randomback(Bird &bird)
{
    int randomed = (rand() % 90) - 45;
    if (bird.pos.x < (-MAX_X + BIRD_MAX_SIZE)) {
        if ((bird.deg >= 90 && bird.deg <= 180) || (bird.deg >= 180 && bird.deg <= 270)) {
            bird.deg = ((bird.deg + 180 + randomed) % 360);
        }
    } else if (bird.pos.y > (MAX_Y - BIRD_MAX_SIZE))  {
        if ((bird.deg >= 90 && bird.deg <= 180) || (bird.deg > 0 && bird.deg < 90)) {
            bird.deg = ((bird.deg + 180 + randomed) % 360);
        }
    } else if (bird.pos.y < (-MAX_Y + BIRD_MAX_SIZE)) {
        if ((bird.deg >= 180 && bird.deg <= 270) || (bird.deg >= 270 && bird.deg <= 360)) {
            bird.deg = ((bird.deg + 180 + randomed) % 360);
        }
    } else if (bird.pos.x > (MAX_X - BIRD_MAX_SIZE)) {
        if ((bird.deg >= 0 && bird.deg <= 90) || (bird.deg >= 270 && bird.deg <= 360)) {
            bird.deg = ((bird.deg + 180 + randomed) % 360);
        }
    }
}

/**
 * If bird touch the wall then make him turn over 90° in natural direction
 */
void bird_handle_world_geometric(Bird &bird)
{
    if (bird.pos.x < (-MAX_X + BIRD_MAX_SIZE)) {
        if (bird.deg >= 90 && bird.deg <= 180) {
            bird.deg = (bird.deg - 90) % 360;
        } else if (bird.deg >= 180 && bird.deg <= 270) {
            bird.deg = (bird.deg + 90) % 360;
        }
    } else if (bird.pos.y > (MAX_Y - BIRD_MAX_SIZE))  {
        if (bird.deg >= 90 && bird.deg <= 180) {
            bird.deg = (bird.deg + 90) % 360;
        } else if (bird.deg > 0 && bird.deg < 90) {
            bird.deg = 360 + (bird.deg - 90);
        }
    } else if (bird.pos.y < (-MAX_Y + BIRD_MAX_SIZE)) {
        if (bird.deg >= 180 && bird.deg <= 270) {
            bird.deg = (bird.deg - 90) % 360;
        } else if (bird.deg >= 270 && bird.deg <= 360) {
            bird.deg = (bird.deg + 90) % 360;
        }
    } else if (bird.pos.x > (MAX_X - BIRD_MAX_SIZE)) {
        if (bird.deg >= 0 && bird.deg <= 90) {
            bird.deg = (bird.deg + 90) % 360;
        } else if (bird.deg >= 270 && bird.deg <= 360) {
            bird.deg = (bird.deg - 90) % 360;
        }
    }
}

/**
 * A world with a end but no border, like the pacman one
 */
void bird_handle_world_infinite(Bird &bird)
{
    if (bird.pos.x < -MAX_X)
        bird.pos.x += MAX_X * 2;
    if (bird.pos.x > MAX_X)
        bird.pos.x -= MAX_X * 2;
    if (bird.pos.y < -MAX_Y)
        bird.pos.y += MAX_Y * 2;
    if (bird.pos.y > MAX_Y)
        bird.pos.y -= MAX_Y * 2;
}

/**
 * Work against wall problems if collision w/ a wall, see above
 */
void bird_handle_world(BirdGroup &birds)
{
    for (int i = 0; i < birds.nb; i++) {
        if (world == 0) {
            bird_handle_world_infinite(birds.bird[i]);
        } else if (world == 1) {
            bird_handle_world_geometric(birds.bird[i]);
        } else if (world == 2) {
            bird_handle_world_randomback(birds.bird[i]);
        }
    }
}

/**
 * Randomly move the bird orientation +/- 10°
 */
void bird_random_life(BirdGroup &birds)
{
    for (int i = 0; i < birds.nb; i++) {
        int randomed = rand() % 180;
        if (randomed <= 20) {
            birds.bird[i].deg = (birds.bird[i].deg + (randomed - 10)) % 360;
            if (birds.bird[i].deg < 0) {
                birds.bird[i].deg = 360 + birds.bird[i].deg;
            }
        }
    }
}

Complex bird_repulsion(BirdGroup &birds, int eval)
{
    Complex repulsion = complex_init_cartesian(0, 0);
    int repulsion_count;

    for (int i = 0; i < birds.nb; i++) {
        float dist = complex_get_distance_diff(birds.bird[i].pos, birds.bird[eval].pos);
        if (dist > 0 && dist < repulsion_field.quantity) {
            repulsion = repulsion + (complex_normalize(birds.bird[eval].pos - birds.bird[i].pos) / dist);
            repulsion_count++;
        }
    }
    if (repulsion_count > 0) {
        repulsion = repulsion / (float)repulsion_count;
    }
    if (complex_get_radius(repulsion) > 0) {
        repulsion = (complex_normalize(repulsion) * velocity_weight.quantity) - birds.bird[eval].velocity;
        repulsion = complex_stage(repulsion, physical_weight.quantity);
    }
    return (repulsion);
}

Complex bird_alignement(BirdGroup &birds, int eval)
{
    Complex alignement = complex_init_cartesian(0, 0);
    int alignement_count;

    for (int i = 0; i < birds.nb; i++) {
        float dist = complex_get_distance_diff(birds.bird[i].pos, birds.bird[eval].pos);
        if (dist > 0 && dist < alignement_field.quantity) {
            alignement = alignement + birds.bird[i].velocity;
            alignement_count++;
        }
    }
    if (alignement_count > 0) {
        alignement = complex_normalize(alignement / (float)alignement_count) * velocity_weight.quantity;
        alignement = complex_stage(alignement - birds.bird[eval].velocity, physical_weight.quantity);
    }
    return (alignement);
}

Complex bird_cohesion(BirdGroup &birds, int eval)
{
    Complex cohesion = complex_init_cartesian(0, 0);
    int cohesion_count;

    for (int i = 0; i < birds.nb; i++) {;
        float dist = complex_get_distance_diff(birds.bird[i].pos, birds.bird[eval].pos);
        if (dist > 0 && dist < cohesion_field.quantity) {;
            cohesion = cohesion + birds.bird[i].pos;
            cohesion_count++;
        }
    }
    if (cohesion_count > 0) {
        cohesion = cohesion / (float)cohesion_count;
        cohesion = complex_init_polar(0, 0) - cohesion;
        cohesion = (complex_normalize(cohesion) * velocity_weight.quantity);
        birds.bird[eval].acc = cohesion - birds.bird[eval].velocity;
        birds.bird[eval].acc = complex_stage(birds.bird[eval].acc, physical_weight.quantity);
    }
    return (cohesion);
}

/**
 * The core of the simulation purpose
 *   - repulsion  (< 20) with others if they are rather near
 *   - alignement (< 50) with others if they are rather near
 *   - cohesion   (< 20) with others if they are rather far
 * All of the update isn't well defined, in fact it's a heap of scalar which we can modulate to obtain more or less a group
 * I (actually) don't think that the current parameters are the best one
 */
void bird_update(BirdGroup &birds)
{
    for (int i = 0; i < birds.nb; i++) {
        Complex repulsion = bird_repulsion(birds, i);
        Complex alignement = bird_alignement(birds, i);
        Complex cohesion = bird_cohesion(birds, i);

        repulsion = repulsion * repulsion_weight.quantity;
        alignement = alignement * alignement_weight.quantity;
        cohesion  = cohesion * cohesion_weight.quantity; 

        birds.bird[i].acc = birds.bird[i].acc + repulsion;
        birds.bird[i].acc = birds.bird[i].acc + alignement;
        birds.bird[i].acc = birds.bird[i].acc + cohesion;
        
        birds.bird[i].acc = birds.bird[i].acc * acceleration_weight.quantity;
        if (grouping) {
            birds.bird[i].velocity = complex_stage(birds.bird[i].velocity + birds.bird[i].acc, velocity_weight.quantity);
            birds.bird[i].pos = birds.bird[i].pos + birds.bird[i].velocity * velocity_weight.quantity;
        } else {
            birds.bird[i].velocity = complex_init_polar(velocity_weight.quantity, birds.bird[i].deg);
            birds.bird[i].pos = birds.bird[i].pos + birds.bird[i].velocity;
        }
        birds.bird[i].acc = birds.bird[i].acc * 0;
    }
}

/**
 * Handle the user input
 */
void handle_input(BirdGroup &birds)
{
    if (isKeyPressed(SDLK_RETURN)) {
        for (int i = 0; i < BIRD_BY_GROUP; i++) {
            birds.bird[i].pos.x = 0;
            birds.bird[i].pos.y = 0;
        }
    }
    if (isKeyPressed(SDLK_SPACE))
        updating = !updating;

    if (isKeyPressed(SDLK_w) && !grouping)
        world = (world + 1) % 3;
    if (isKeyPressed(SDLK_r))
        randoming = !randoming;
    if (isKeyPressed(SDLK_g)) {
        grouping = !grouping;
        if (grouping)
            world = 0;
    }

    if (isKeyPressed(SDLK_a)) {
        input_focus = &propagation_probability;
    }
    if (isKeyPressed(SDLK_b)) {
        input_focus = &radius_propagation;
    }
    if (isKeyPressed(SDLK_c)) {
        input_focus = &immunity_weight;
    }
    if (isKeyPressed(SDLK_d)) {
        input_focus = &deathrate;
    }

    if (isKeyPressed(SDLK_3))
        input_focus = &cohesion_weight;
    if (isKeyPressed(SDLK_2))
        input_focus = &alignement_weight;
    if (isKeyPressed(SDLK_1))
        input_focus = &repulsion_weight;
    if (isKeyPressed(SDLK_6))
        input_focus = &cohesion_field;
    if (isKeyPressed(SDLK_5))
        input_focus = &alignement_field;
    if (isKeyPressed(SDLK_4))
        input_focus = &repulsion_field;
    if (isKeyPressed(SDLK_9))
        input_focus = &physical_weight;
    if (isKeyPressed(SDLK_8))
        input_focus = &acceleration_weight;
    if (isKeyPressed(SDLK_7))
        input_focus = &velocity_weight;

    if (isKeyPressed(SDLK_UP))
        input_focus->quantity += input_focus->mod;
    if (isKeyPressed(SDLK_DOWN))
        if (input_focus->quantity > input_focus->mod)
            input_focus->quantity -= input_focus->mod;

}

void dynamic_information_focus(int x, int y, Modulate *cur)
{
    char buf[10];

    if (cur == input_focus) {
        color(255, 0, 0, 255);
    } else {
        color(255, 179, 0, 255);
    }
    if (cur->mod >= 1)
        snprintf(buf, 10, "%.0f", cur->quantity);
    else if (cur->mod >= 0.1)
        snprintf(buf, 10, "%.1f", cur->quantity);
    else
        snprintf(buf, 10, "%.2f", cur->quantity);
    print(x, y, buf);
    color(120, 144, 156, 255);
}

/**
 * Display dynamic informations
 */
void dynamic_simulation_information(void)
{
    color(120, 144, 156, 255);
    print(0, 0,  "Randoming (r):");
    color(255, 179, 0, 255);
    print(110, 0, randoming ? "True" : "False");
    color(120, 144, 156, 255);
    print(0, 16,  "Grouping (g):");
    color(255, 179, 0, 255);
    print(110, 16, grouping ? "True" : "False");
    color(120, 144, 156, 255);
    print(0, 16 * 2, "World (w):");
    color(255, 179, 0, 255);
    if (world == 0) {
        print(110, 16 * 2, "Infinite");
    } else if (world == 1) {
        print(110, 16 * 2, "Geometric");
    } else if (world == 2) {
        print(110, 16 * 2, "Random");
    }

    color(120, 144, 156, 255);
    print(180, 0,  "cohesion weight (3):");
    dynamic_information_focus(180 + 165, 0, &cohesion_weight);
    print(180, 16, "alignement weight (2):");
    dynamic_information_focus(180 + 165, 16, &alignement_weight);
    print(180, 16 * 2, "repulsion weight (1):");
    dynamic_information_focus(180 + 165, 16 * 2, &repulsion_weight);
    print(400, 0, "cohesion field (6):");
    dynamic_information_focus(350 + 200, 0, &cohesion_field);
    print(400, 16, "alignement field (5):");
    dynamic_information_focus(350 + 200, 16, &alignement_field);
    print(400, 16 * 2, "repulsion field (4):");
    dynamic_information_focus(350 + 200, 16 * 2, &repulsion_field);
    print(600, 0, "physical weight (9):");
    dynamic_information_focus(600 + 175, 0, &physical_weight);
    print(600, 16, "acceleration weight (8):");
    dynamic_information_focus(600 + 175, 16, &acceleration_weight);
    print(600, 16 * 2, "velocity weight (7):");
    dynamic_information_focus(600 + 175, 16 * 2, &velocity_weight);

}

void dynamic_health_information(BirdGroup &birds)
{
    color(120, 144, 156, 255);
    print(0, MAX_Y * 2 - 16 - 2, "Clean: ");
    dynamic_information_focus(80, MAX_Y * 2 - 16 - 2, &bird_clean);
    print(0, MAX_Y * 2 - 16 * 2 - 2, "Infected: ");
    dynamic_information_focus(80, MAX_Y * 2 - 16 * 2 - 2, &bird_infected);
    print(0, MAX_Y * 2 - 16 * 3 - 2, "Immune: ");
    dynamic_information_focus(80, MAX_Y * 2 - 16 * 3 - 2, &bird_immune);
    print(0, MAX_Y * 2 - 16 * 4 - 2, "Dead: ");
    dynamic_information_focus(80, MAX_Y * 2 - 16 * 4 - 2, &bird_dead);

    print(0 + 135, MAX_Y * 2 - 16 * 1 - 2, "R0 (a): ");
    dynamic_information_focus(110 + 135, MAX_Y * 2 - 16 * 1 - 2, &propagation_probability);
    print(0 + 135, MAX_Y * 2 - 16 * 2 - 2, "Radius (b): ");
    dynamic_information_focus(110 + 135, MAX_Y * 2 - 16 * 2 - 2, &radius_propagation);
    print(0 + 135, MAX_Y * 2 - 16 * 3 - 2, "Immunity (c): ");
    dynamic_information_focus(110 + 135, MAX_Y * 2 - 16 * 3 - 2, &immunity_weight);
    print(0 + 135, MAX_Y * 2 - 16 * 4 - 2, "deathrate (d): ");
    dynamic_information_focus(110 + 135, MAX_Y * 2 - 16 * 4 - 2, &deathrate);

}

bool propagation_random(float max, float proba)
{
    return ((rand() % (int)max) < (int)(proba * max));
}

void update_health(BirdGroup &birds)
{
    for (int i = 0; i < birds.nb; i++) {
        // IMMUNE: make it simple and keep immunity forever, in fact it's obv not the case
        // CLEAN: clean bird make nothing
        // DEAD: dead bird can't revive
        // INFECTED: infect others and die if no chance :(
        if (birds.bird[i].health.state == INFECTED) {
            for (int j = 0; j < birds.nb; j++) {
                if (birds.bird[j].health.state == CLEAN || (birds.bird[j].health.state == IMMUNE && !propagation_random(100, immunity_weight.quantity))) {
                    float dist = complex_get_distance_diff(birds.bird[i].pos, birds.bird[j].pos);
                    if (dist < radius_propagation.quantity && propagation_random(100, propagation_probability.quantity / 5)) {
                            if (birds.bird[j].health.state == CLEAN)
                                bird_clean.quantity -= 1;
                            else if (birds.bird[j].health.state == IMMUNE)
                                bird_immune.quantity -= 1;
                            birds.bird[j].health.state = INFECTED;
                            bird_infected.quantity += 1;
                            birds.bird[j].color = color_init(255, 0, 0, 255);
                    }
                }
            }
            if (propagation_random(100, deathrate.quantity)) {
                birds.bird[i].health.state = DEAD;
                bird_infected.quantity -= 1;
                bird_dead.quantity += 1;
            } else if (birds.bird[i].health.infected_clock++ == 100) {
                birds.bird[i].health.infected_clock = 0;
                birds.bird[i].health.state = IMMUNE;
                bird_immune.quantity += 1;
                bird_infected.quantity -= 1;
                birds.bird[i].color = color_init(0, 255, 0, 255);
            }       
        }
    }
}

int main(void)
{
    int clock = 0;
    BirdGroup birds;

    winInit("Fouloscopia", MAX_X * 2, MAX_Y * 2);
    backgroundColor(0, 0, 0, 0);
    color(255, 255, 255, 255);
    fontSize(16);
    srand(time(NULL));

    bird_init(birds, BIRD_BY_GROUP);

    birds.bird[rand() % BIRD_BY_GROUP].health.state = INFECTED;

    while (!isKeyPressed(SDLK_ESCAPE)) {
        winClear();
        if (updating) {
            if (randoming) {
                bird_random_life(birds); // the life being unpredictable, put a little bit of random
            }
            bird_update(birds); // update the bird position
            bird_handle_world(birds); // handle world depending on given rules
            delay(1);
            if ((int)SDL_GetTicks() / 400 > clock) { // ~ 2.5 times by second
                update_health(birds);
                clock++;
            }
        }
        bird_draw(birds); // finally draw
        dynamic_health_information(birds); // display bird health information
        dynamic_simulation_information(); // display the dynamic information for the user
        handle_input(birds); // handle user inputs
        winDisplay();
    }

    winQuit();
    return (EXIT_SUCCESS);
}