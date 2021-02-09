# Fouloscopia

Fouloscopia is a little Boid-like implementation with run-time parametring.

The project is lead using the `GrAPiC` graphic library provided by [Alexandre Meyer](https://perso.liris.cnrs.fr/alexandre.meyer/grapic/html/index.html).

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

> make build target=fouloscopia

run it;

> ./fouloscopia.bin

clean the *all* the build:

> make clean

## About

Runtime key trigger.

 Key     | Description                              | Qunatity Involved
---------|------------------------------------------|---------------------
`\SPACE` | Pause the bird update                    | 
`\ENTRY` | Reset all the bird position to 0         | 
`\ESC`   | Quit the program                         |
`\UP`    | Increase the focused quantity            | Increase by 0.2
`\DOWN`  | Decrease the focused quantity            | Decrease by 0.2
`w`      | modify bird world interaction            | Geometric or randomback or infinite
`g`      | modify grouping behavior                 | boolean
`r`      | modify randoming behavior                | boolean
`1`      | focuse the repulsion weight quantity     | float
`2`      | focuse the alignement weight quantity    | float
`3`      | focuse the cohesion weight quantity      | float
`4`      | focuse the repulsion field quantity      | float
`5`      | focuse the alignement field quantity     | float
`6`      | focuse the cohesion field quantity       | float
`7`      | focuse the velocity weight quantity      | float
`8`      | focuse the acceleration weight quantity  | float
`9`      | focuse the physical weight quantity      | float

## Sampling

![Sample](doc/sample.gif)

### Epilogue

Feel free to fork, use, improve.










