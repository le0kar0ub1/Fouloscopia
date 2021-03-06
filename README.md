# Fouloscopia

Fouloscopia is a little Boids-like implementation with runtime parametring. On top of that a disease propagation is implemented, with also, runtime parametring.

The project is led using the `GrAPiC` graphic library provided by [Alexandre Meyer](https://perso.liris.cnrs.fr/alexandre.meyer/grapic/html/index.html).

## Dependencies

* make
* git
* curl

## Install

This installer is working for Linux system only.

> git clone https://github.com/le0kar0ub1/Fouloscopia.git && cd Fouloscopia && make install

Local installation handled:
  * SDL2
  * SDL2_image
  * SDL2_ttf
  * GrAPiC

## Build & Run

Build the executable:

> make [build]

run it;

> ./fouloscopia.bin

clean the the build:

> make clean

## About

Runtime basic key trigger.

 Key     | Description
---------|------------------------------------------
`\MOUSE` | Press left button of the mouse to drop new clean bird at the current position
`\SPACE` | Pause the boid update
`\ENTRY` | Reset all the boid position to 0 (middle of the map)
`\ESC`   | Quit the program
`\UP`    | Increase the focused quantity
`\DOWN`  | Decrease the focused quantity


Runtime boid behavior.

 Key     | Description                              | Quantity Involved
---------|------------------------------------------|---------------------
`w`      | modify boid world interaction            | Geometric or randomback or infinite
`g`      | modify grouping behavior                 | boolean
`r`      | modify randoming behavior                | boolean
`1`      | focus the repulsion weight quantity      | float
`2`      | focus the alignment weight quantity      | float
`3`      | focus the cohesion weight quantity       | float
`4`      | focus the repulsion field quantity       | float
`5`      | focus the alignment field quantity       | float
`6`      | focus the cohesion field quantity        | float
`7`      | focus the velocity weight quantity       | float
`8`      | focus the acceleration weight quantity   | float
`9`      | focus the physical weight quantity       | float

Runtime boid health.

 Key     | Description                                                      | Quantity Involved
---------|------------------------------------------------------------------|---------------------
`a`      | focus the r0 quantity                                            | float
`b`      | focus the radius quantity (minimal distance for propagation)     | float
`c`      | focus the immunity weight quantity between 0 and 1               | float
`d`      | focus the death rate quantity between 0 and 1                    | float
`e`      | focus the infection duration                                     | float

## Sampling

![Sample](doc/sample.gif)

### Epilogue

Feel free to fork, use, improve.










