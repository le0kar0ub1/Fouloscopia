# include "Fouloscopia.hpp"
# include "Grapic.h"

Fouloscopia::Fouloscopia()
{
    int nb = 0;

    boids = new std::vector<Boid *>;
    while (nb < BOID_NUMBER) {
        this->boids->push_back(new Boid());
        nb++;
    }
}

Fouloscopia::~Fouloscopia()
{
    for (auto it = boids->begin(); it != boids->end(); it++)
        delete *it;
    delete boids;
}

void Fouloscopia::init()
{
    winInit("Fouloscopia", MAX_X * 2, MAX_Y * 2);
    backgroundColor(0, 0, 0, 0);
    color(255, 255, 255, 255);
    fontSize(16);
    srand(time(NULL));

    auto boid = this->boids->begin();
    std::advance(boid, rand() % BOID_NUMBER);
    (*boid)->set_health(INFECTED);
    (*boid)->set_color(Color(255, 0, 0, 255));
    boid_infected.val += 1;
    boid_clean.val -= 1;
}

/**
 * Handle the user input
 */
void Fouloscopia::handle_input()
{
    if (isKeyPressed(SDLK_RETURN))
        for (auto it = simulation.boids->begin(); it != simulation.boids->end(); it++)
            (*it)->set_pos(0, 0);
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
        focus = &propagation_probability;
    if (isKeyPressed(SDLK_b))
        focus = &radius_propagation;
    if (isKeyPressed(SDLK_c))
        focus = &immunity_weight;
    if (isKeyPressed(SDLK_d))
        focus = &deathrate;
    if (isKeyPressed(SDLK_e))
        focus = &infection_duration;

    if (isKeyPressed(SDLK_3))
        focus = &cohesion_weight;
    if (isKeyPressed(SDLK_2))
        focus = &alignment_weight;
    if (isKeyPressed(SDLK_1))
        focus = &repulsion_weight;
    if (isKeyPressed(SDLK_6))
        focus = &cohesion_field;
    if (isKeyPressed(SDLK_5))
        focus = &alignment_field;
    if (isKeyPressed(SDLK_4))
        focus = &repulsion_field;
    if (isKeyPressed(SDLK_9))
        focus = &physical_weight;
    if (isKeyPressed(SDLK_8))
        focus = &acceleration_weight;
    if (isKeyPressed(SDLK_7))
        focus = &velocity_weight;

    if (isKeyPressed(SDLK_UP))
        focus->val += focus->mod;
    if (isKeyPressed(SDLK_DOWN))
        if (focus->val >= focus->mod)
            focus->val -= focus->mod;

}

/**
 * Print a quantity, particularly print it in red if focused
 */
void Fouloscopia::dynamic_information_focus(int x, int y, Quantity *cur)
{
    char buf[10];

    if (cur == focus) {
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
void Fouloscopia::dynamic_simulation_information(void)
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
    if (world == WORLD_INFINITE) {
        print(110, 16 * 2, "Infinite");
    } else if (world == WORLD_GEOMETRIC) {
        print(110, 16 * 2, "Geometric");
    } else if (world == WORLD_RANDOMBACK) {
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
void Fouloscopia::dynamic_health_information(void)
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

void Fouloscopia::run()
{
    int clock = 0;
    bool healthtrigger = false;

    init();

    while (!isKeyPressed(SDLK_ESCAPE)) {
        winClear();
        if ((int)SDL_GetTicks() / (1000 / HEALTH_STEP) > clock) { // ~ 2 times by second
            healthtrigger = true;
            clock++;
        }
        for (auto it = simulation.boids->begin(); it != simulation.boids->end(); it++) {
            if (updating) {
                if (randoming) {
                    (*it)->random_life(); // the life being unpredictable, put a little bit of random
                }
                (*it)->update_position(); // update the boid position
                (*it)->handle_world(); // handle world depending on given rules
                if (healthtrigger) {
                    (*it)->update_health();
                }
            }
            (*it)->draw();
        }
        if (healthtrigger) {
            healthtrigger = false;
        }
        dynamic_health_information(); // display boid health information
        dynamic_simulation_information(); // display the dynamic information for the user
        handle_input(); // handle user inputs
        winDisplay();
        // delay(10);
    }

    winQuit();
}