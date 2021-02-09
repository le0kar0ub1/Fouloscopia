# include <Grapic.h>
# include <iostream>

using namespace std;
using namespace grapic;

# define MAX_X 500 // window size / 2 (this is a complex grid, you know)
# define MAX_Y 500 // window size / 2 (this is a complex grid, you know)

# define BIRD_BY_GROUP 300 // number of bird by group

# define BIRD_MAX_SIZE 10 // size of one bird

# define BIRD_SPEED 3.5 // speed of the birds
# define BIRD_PHYS  1 // weight of the rules, don't touch me

bool grouping = false; // must the bird make group
int  world = 0; // 0 == infinite | 1 == geometric | 2 == random back
bool updating = true; // if space, pause the update
bool randoming = true; // put random in brid move

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

Complex complex_init_cart(float x, float y)
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

float complex_get_angle_diff(Complex a, Complex b)
{
    double base;

    if ((a.x == 0 && a.y == 0) || (b.x == 0 && b.y == 0)) {
        return (0.0);
    }
    base = (a.x * b.x + a.y * b.y) / (complex_get_radius(a) * complex_get_radius(b));
    if (base <= -1) {
        return (M_PI);
    } else if (base >= 1) {
        return (0);
    }
    return ((float)acos(base));
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
void bird_init(BirdGroup &birds, int nb, Color color)
{
    for (int i = 0; i < nb; i++) {
        birds.bird[i].pos.x = 0;
        birds.bird[i].pos.y = 0;
        birds.bird[i].deg = rand() % 360;
        birds.bird[i].color = color;
        birds.bird[i].velocity = complex_init_cart(rand() % 3 - 2, rand() % 3 - 2);
        birds.bird[i].acc = complex_init_cart(0, 0);
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

# define REPULSION_FIELD  20
Complex bird_repulsion(BirdGroup &birds, int eval)
{
    Complex repulsion = complex_init_cart(0, 0);
    int repulsion_count;

    for (int i = 0; i < birds.nb; i++) {
        float dist = complex_get_distance_diff(birds.bird[i].pos, birds.bird[eval].pos);
        if (dist > 0 && dist < REPULSION_FIELD) {
            repulsion = repulsion + (complex_normalize(birds.bird[eval].pos - birds.bird[i].pos) / dist);
            repulsion_count++;
        }
    }
    if (repulsion_count > 0) {
        repulsion = repulsion / (float)repulsion_count;
    }
    if (complex_get_radius(repulsion) > 0) {
        repulsion = (complex_normalize(repulsion) * BIRD_SPEED) - birds.bird[eval].velocity;
        repulsion = complex_stage(repulsion, BIRD_PHYS);
    }
    return (repulsion);
}

# define ALIGNEMENT_FIELD 50
Complex bird_alignement(BirdGroup &birds, int eval)
{
    Complex alignement = complex_init_cart(0, 0);
    int alignement_count;

    for (int i = 0; i < birds.nb; i++) {
        float dist = complex_get_distance_diff(birds.bird[i].pos, birds.bird[eval].pos);
        if (dist > 0 && dist < ALIGNEMENT_FIELD) {
            alignement = alignement + birds.bird[i].velocity;
            alignement_count++;
        }
    }
    if (alignement_count > 0) {
        alignement = complex_normalize(alignement / (float)alignement_count) * BIRD_SPEED;
        alignement = complex_stage(alignement - birds.bird[eval].velocity, BIRD_PHYS);
    }
    return (alignement);
}

# define COHESION_FIELD   20
Complex bird_cohesion(BirdGroup &birds, int eval)
{
    Complex cohesion = complex_init_cart(0, 0);
    int cohesion_count;

    for (int i = 0; i < birds.nb; i++) {
        float dist = complex_get_distance_diff(birds.bird[i].pos, birds.bird[eval].pos);
        if (dist > 0 && dist < COHESION_FIELD) {
            cohesion = cohesion + birds.bird[i].pos;
            cohesion_count++;
        }
    }
    if (cohesion_count > 0) {
        cohesion = cohesion / (float)cohesion_count;
        cohesion = complex_init_polar(0, 0) - cohesion;
        cohesion = (complex_normalize(cohesion) * BIRD_SPEED);
        birds.bird[eval].acc = cohesion - birds.bird[eval].velocity;
        birds.bird[eval].acc = complex_stage(birds.bird[eval].acc, BIRD_PHYS);
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

        repulsion = repulsion * 10.0;//1.5;
        cohesion  = cohesion * 0.2; 

        birds.bird[i].acc = birds.bird[i].acc + repulsion;
        birds.bird[i].acc = birds.bird[i].acc + alignement;
        birds.bird[i].acc = birds.bird[i].acc + cohesion;
        
        birds.bird[i].acc = birds.bird[i].acc;// * 0.4;
        if (grouping) {
            birds.bird[i].velocity = complex_stage(birds.bird[i].velocity + birds.bird[i].acc, BIRD_SPEED);
            birds.bird[i].pos = birds.bird[i].pos + birds.bird[i].velocity * BIRD_SPEED;
        } else {
            birds.bird[i].velocity = complex_init_polar(BIRD_SPEED, birds.bird[i].deg);
            birds.bird[i].pos = birds.bird[i].pos + birds.bird[i].velocity;
        }
        birds.bird[i].acc = birds.bird[i].acc * 0;
    }
}

int main(void)
{
    BirdGroup birds;

    winInit("Fouloscopia", MAX_X * 2, MAX_Y * 2);
    backgroundColor(0, 0, 0, 0);
    color(255, 255, 255, 255);
    fontSize(14);
    srand(time(NULL));

    Color color = {
        .r = 255,
        .g = 255,
        .b = 255,
        .a = 255
    };
    bird_init(birds, BIRD_BY_GROUP, color);

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
        print(0, 0,  randoming ?         "Randoming (r): True"      : "Randoming (r): False");
        print(0, 14, grouping ?          "Grouping  (g): True"      : "Grouping  (g): False");
        if (world == 0) {
            print(0, 28, "Collision (c): Infinite");
        } else if (world == 1) {
            print(0, 28, "Collision (c): Geometric");
        } else if (world == 2) {
            print(0, 28, "Collision (c): Random");
        }
        if (isKeyPressed(SDLK_g)) {
            grouping = !grouping;
            if (grouping)
                world = 0;
        }
        if (isKeyPressed(SDLK_SPACE)) {
            updating = !updating;
        }
        if (isKeyPressed(SDLK_c) && !grouping) {
            world = (world + 1) % 3;
                
        }
        if (isKeyPressed(SDLK_r)) {
            randoming = !randoming;
        }
        winDisplay();
    }

    winQuit();
    return (EXIT_SUCCESS);
}