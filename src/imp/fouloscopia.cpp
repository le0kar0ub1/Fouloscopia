# include <iostream>
# include <string>
# include "Complex.hpp"
# include "Boid.hpp"
# include "fouloscopia.hpp"

bool grouping = true; // if true, the boids try to group
int  world = 0; // 0 == infinite | 1 == geometric | 2 == random back
bool updating = true; // if true, pause the update
bool randoming = true; // if true, put random in brid move

Quantity boid_size = {.val = 10, .mod = 1}; // size of one boid

Quantity repulsion_field  = {.val = 20, .mod = 1}; // field of action of the behavior
Quantity alignment_field = {.val = 50, .mod = 1}; // field of action of the behavior
Quantity cohesion_field   = {.val = 20, .mod = 1}; // field of action of the behavior

Quantity acceleration_weight = {.val = 1, .mod = 0.2};
Quantity velocity_weight     = {.val = 6, .mod = 0.2};
Quantity physical_weight     = {.val = 1.0, .mod = 0.2};

Quantity repulsion_weight  = {.val = 10.0, .mod = 0.5};
Quantity alignment_weight = {.val = 1.0, .mod = 0.5};
Quantity cohesion_weight   = {.val = 0.2, .mod = 0.5};

Quantity propagation_probability = {.val = 2.5, .mod = 0.2};
Quantity radius_propagation = {.val = 20, .mod = 1};
Quantity immunity_weight = {.val = 0.90, .mod = 0.02};
Quantity deathrate = {.val = 0.4, .mod = 0.02};
Quantity infection_duration = {.val = 20, .mod = 2 };

Quantity boid_clean = {.val = BOID_NUMBER, .mod = 0};
Quantity boid_infected = {.val = 0, .mod = 0};
Quantity boid_immune = {.val = 0, .mod = 0};
Quantity boid_dead = {.val = 0, .mod = 0};

Quantity *input_focus = &propagation_probability;

/**
 * Handle the user input
 */
void handle_input(BoidGroup &boids)
{
    if (isKeyPressed(SDLK_RETURN))
        for (int i = 0; i < BOID_NUMBER; i++) {
            boids.boid[i].pos.set(0, 0);
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

    if (isKeyPressed(SDLK_a))
        input_focus = &propagation_probability;
    if (isKeyPressed(SDLK_b))
        input_focus = &radius_propagation;
    if (isKeyPressed(SDLK_c))
        input_focus = &immunity_weight;
    if (isKeyPressed(SDLK_d))
        input_focus = &deathrate;
    if (isKeyPressed(SDLK_e))
        input_focus = &infection_duration;

    if (isKeyPressed(SDLK_3))
        input_focus = &cohesion_weight;
    if (isKeyPressed(SDLK_2))
        input_focus = &alignment_weight;
    if (isKeyPressed(SDLK_1))
        input_focus = &repulsion_weight;
    if (isKeyPressed(SDLK_6))
        input_focus = &cohesion_field;
    if (isKeyPressed(SDLK_5))
        input_focus = &alignment_field;
    if (isKeyPressed(SDLK_4))
        input_focus = &repulsion_field;
    if (isKeyPressed(SDLK_9))
        input_focus = &physical_weight;
    if (isKeyPressed(SDLK_8))
        input_focus = &acceleration_weight;
    if (isKeyPressed(SDLK_7))
        input_focus = &velocity_weight;

    if (isKeyPressed(SDLK_UP))
        input_focus->val += input_focus->mod;
    if (isKeyPressed(SDLK_DOWN))
        if (input_focus->val > input_focus->mod)
            input_focus->val -= input_focus->mod;

}

/**
 * Print a quantity, particularly print it in red if focused
 */
void dynamic_information_focus(int x, int y, Quantity *cur)
{
    char buf[10];

    if (cur == input_focus) {
        color(255, 0, 0, 255);
    } else {
        color(255, 179, 0, 255);
    }
    if (cur->mod >= 1 || cur->mod == 0)
        snprintf(buf, 10, "%.0f", cur->val);
    else if (cur->mod >= 0.1)
        snprintf(buf, 10, "%.1f", cur->val);
    else
        snprintf(buf, 10, "%.2f", cur->val);
    print(x, y, buf);
    color(120, 144, 156, 255);
}

/**
 * Display dynamic informations about the boids behavior and the world
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
    print(190, 0,  "cohesion weight (3):");
    dynamic_information_focus(190 + 165, 0, &cohesion_weight);
    print(190, 16, "alignment weight (2):");
    dynamic_information_focus(190 + 165, 16, &alignment_weight);
    print(190, 16 * 2, "repulsion weight (1):");
    dynamic_information_focus(190 + 165, 16 * 2, &repulsion_weight);
    print(400, 0, "cohesion field (6):");
    dynamic_information_focus(350 + 200, 0, &cohesion_field);
    print(400, 16, "alignment field (5):");
    dynamic_information_focus(350 + 200, 16, &alignment_field);
    print(400, 16 * 2, "repulsion field (4):");
    dynamic_information_focus(350 + 200, 16 * 2, &repulsion_field);
    print(600, 0, "physical weight (9):");
    dynamic_information_focus(600 + 175, 0, &physical_weight);
    print(600, 16, "acceleration weight (8):");
    dynamic_information_focus(600 + 175, 16, &acceleration_weight);
    print(600, 16 * 2, "velocity weight (7):");
    dynamic_information_focus(600 + 175, 16 * 2, &velocity_weight);

}

/**
 * Dysplay dynamic information about the boid health
 */
void dynamic_health_information(void)
{
    color(255, 255, 255, 255);
    rectangleFill(0, MAX_Y * 2 - 16 - 2, 16, MAX_Y * 2 - 16 - 2 + 16);
    color(120, 144, 156, 255);
    print(0 + 20, MAX_Y * 2 - 16 - 2, "Clean: ");
    dynamic_information_focus(80 + 20, MAX_Y * 2 - 16 - 2, &boid_clean);

    color(255, 0, 0, 255);
    rectangleFill(0, MAX_Y * 2 - 16 * 2 - 2, 16, MAX_Y * 2 - 16 * 2 - 2 + 16);
    color(120, 144, 156, 255);
    print(0 + 20, MAX_Y * 2 - 16 * 2 - 2, "Infected: ");
    dynamic_information_focus(80 + 20, MAX_Y * 2 - 16 * 2 - 2, &boid_infected);

    color(0, 255, 0, 255);
    rectangleFill(0, MAX_Y * 2 - 16 * 3 - 2, 16, MAX_Y * 2 - 16 * 3 - 2 + 16);
    color(120, 144, 156, 255);
    print(0 + 20, MAX_Y * 2 - 16 * 3 - 2, "Immune: ");
    dynamic_information_focus(80 + 20, MAX_Y * 2 - 16 * 3 - 2, &boid_immune);

    color(0, 0, 0, 0);
    rectangleFill(0, MAX_Y * 2 - 16 * 4 - 2, 16, MAX_Y * 2 - 16 * 4 - 2 + 16);
    color(120, 144, 156, 255);
    print(0 + 20, MAX_Y * 2 - 16 * 4 - 2, "Dead: ");
    dynamic_information_focus(80 + 20, MAX_Y * 2 - 16 * 4 - 2, &boid_dead);

    print(0 + 135, MAX_Y * 2 - 16 * 1 - 2, "R0 (a): ");
    dynamic_information_focus(110 + 135, MAX_Y * 2 - 16 * 1 - 2, &propagation_probability);
    print(0 + 135, MAX_Y * 2 - 16 * 2 - 2, "Radius (b): ");
    dynamic_information_focus(110 + 135, MAX_Y * 2 - 16 * 2 - 2, &radius_propagation);
    print(0 + 135, MAX_Y * 2 - 16 * 3 - 2, "Immunity (c): ");
    dynamic_information_focus(110 + 135, MAX_Y * 2 - 16 * 3 - 2, &immunity_weight);
    print(0 + 135, MAX_Y * 2 - 16 * 4 - 2, "death rate (d): ");
    dynamic_information_focus(110 + 135, MAX_Y * 2 - 16 * 4 - 2, &deathrate);

    print(110 + 135 + 50, MAX_Y * 2 - 16 * 1 - 2, "Infection duration (e): ");
    dynamic_information_focus(110 + 135 + 50 + 170, MAX_Y * 2 - 16 * 1 - 2, &infection_duration);

}

int main(void)
{
    int clock = 0;
    int randomed;
    BoidGroup boids;

    winInit("Fouloscopia", MAX_X * 2, MAX_Y * 2);
    backgroundColor(0, 0, 0, 0);
    color(255, 255, 255, 255);
    fontSize(16);
    srand(time(NULL));

    boid_init(boids, BOID_NUMBER);

    randomed = rand() % BOID_NUMBER;
    boids.boid[randomed].health.state = INFECTED; // infect a random one
    boids.boid[randomed].color = Color(255, 0, 0, 255);
    boid_clean.val -= 1;
    boid_infected.val += 1;


    while (!isKeyPressed(SDLK_ESCAPE)) {
        winClear();
        if (updating) {
            if (randoming) {
                boid_random_life(boids); // the life being unpredictable, put a little bit of random
            }
            boid_update(boids); // update the boid position
            boid_handle_world(boids); // handle world depending on given rules
            delay(10);
            if ((int)SDL_GetTicks() / (1000 / HEALTH_STEP) > clock) { // ~ 2 times by second
                update_health(boids);
                clock++;
            }
        } else {
            delay(50);
        }
        boid_draw(boids); // finally draw
        dynamic_health_information(); // display boid health information
        dynamic_simulation_information(); // display the dynamic information for the user
        handle_input(boids); // handle user inputs
        winDisplay();
    }

    winQuit();
    return (EXIT_SUCCESS);
}