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

float repulsion_field  = 20;
float alignement_field = 50;
float cohesion_field   = 20;

float acceleration_weight = 1;
float velocity_weight = 3.5;
float physical_weight = 1.0;

float repulsion_weight = 10.0;
float alignement_weight = 1.0;
float cohesion_weight = 0.2;

float *input_focus = &repulsion_field;

struct Color
{
    int r;
    int g;
    int b;
    int a;
};

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

struct Bird
{
    int deg;
    Color color;
    Complex pos;
    Complex velocity;
    Complex acc;
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
    Color color = {
        .r = 255,
        .g = 255,
        .b = 255,
        .a = 255
    };

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
        if (dist > 0 && dist < repulsion_field) {
            repulsion = repulsion + (complex_normalize(birds.bird[eval].pos - birds.bird[i].pos) / dist);
            repulsion_count++;
        }
    }
    if (repulsion_count > 0) {
        repulsion = repulsion / (float)repulsion_count;
    }
    if (complex_get_radius(repulsion) > 0) {
        repulsion = (complex_normalize(repulsion) * velocity_weight) - birds.bird[eval].velocity;
        repulsion = complex_stage(repulsion, physical_weight);
    }
    return (repulsion);
}

Complex bird_alignement(BirdGroup &birds, int eval)
{
    Complex alignement = complex_init_cartesian(0, 0);
    int alignement_count;

    for (int i = 0; i < birds.nb; i++) {
        float dist = complex_get_distance_diff(birds.bird[i].pos, birds.bird[eval].pos);
        if (dist > 0 && dist < alignement_field) {
            alignement = alignement + birds.bird[i].velocity;
            alignement_count++;
        }
    }
    if (alignement_count > 0) {
        alignement = complex_normalize(alignement / (float)alignement_count) * velocity_weight;
        alignement = complex_stage(alignement - birds.bird[eval].velocity, physical_weight);
    }
    return (alignement);
}

Complex bird_cohesion(BirdGroup &birds, int eval)
{
    Complex cohesion = complex_init_cartesian(0, 0);
    int cohesion_count;

    for (int i = 0; i < birds.nb; i++) {;
        float dist = complex_get_distance_diff(birds.bird[i].pos, birds.bird[eval].pos);
        if (dist > 0 && dist < cohesion_field) {;
            cohesion = cohesion + birds.bird[i].pos;
            cohesion_count++;
        }
    }
    if (cohesion_count > 0) {
        cohesion = cohesion / (float)cohesion_count;
        cohesion = complex_init_polar(0, 0) - cohesion;
        cohesion = (complex_normalize(cohesion) * velocity_weight);
        birds.bird[eval].acc = cohesion - birds.bird[eval].velocity;
        birds.bird[eval].acc = complex_stage(birds.bird[eval].acc, physical_weight);
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

        repulsion = repulsion * repulsion_weight;;
        alignement = alignement * alignement_weight;
        cohesion  = cohesion * cohesion_weight; 

        birds.bird[i].acc = birds.bird[i].acc + repulsion;
        birds.bird[i].acc = birds.bird[i].acc + alignement;
        birds.bird[i].acc = birds.bird[i].acc + cohesion;
        
        birds.bird[i].acc = birds.bird[i].acc * acceleration_weight;;
        if (grouping) {
            birds.bird[i].velocity = complex_stage(birds.bird[i].velocity + birds.bird[i].acc, velocity_weight);
            birds.bird[i].pos = birds.bird[i].pos + birds.bird[i].velocity * velocity_weight;
        } else {
            birds.bird[i].velocity = complex_init_polar(velocity_weight, birds.bird[i].deg);
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
        *input_focus += 0.2;
    if (isKeyPressed(SDLK_DOWN))
        if (*input_focus > 0.2)
            *input_focus -= 0.2;

}

void dynamic_information_focus(int x, int y, float *cur)
{
    char buf[10];

    if (cur == input_focus) {
        color(255, 0, 0, 255);
    } else {
        color(255, 179, 0, 255);
    }
    snprintf(buf, 10, "%.1f", *cur);
    print(x, y, buf);
    color(120, 144, 156, 255);
}

/**
 * Display dynamic informations
 */
void dynamic_information(void)
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

int main(void)
{
    BirdGroup birds;

    winInit("Fouloscopia", MAX_X * 2, MAX_Y * 2);
    backgroundColor(0, 0, 0, 0);
    color(255, 255, 255, 255);
    fontSize(16);
    srand(time(NULL));

    bird_init(birds, BIRD_BY_GROUP);

    while (!isKeyPressed(SDLK_ESCAPE)) {
        winClear();
        if (updating) {
            if (randoming) {
                bird_random_life(birds); // the life being unpredictable, put a little bit of random
            }
            bird_update(birds); // update the bird position
            bird_handle_world(birds); // handle world depending on given rules
            delay(1);
        }
        bird_draw(birds); // finally draw
        handle_input(birds); // handle user inputs
        dynamic_information(); // display the dynamic information for the user
        winDisplay();
    }

    winQuit();
    return (EXIT_SUCCESS);
}