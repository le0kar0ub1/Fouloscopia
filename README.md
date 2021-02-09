# IWGrAPiC

Installer for the `GrAPiC` graphic library provided by [Alexandre Meyer](https://perso.liris.cnrs.fr/alexandre.meyer/grapic).

The goal of this installer is to minimize the side effects and the user job while installing and using a little library like GrAPiC.

This installer is working for Linux system only.

## Philosophy

When you want to create some projects with the library `GrAPiC` just clone this repository, install all the dependencies with a command and create your projects inside this repository.

So, you don't have to install permanently the library.

## Dependencies

* make
* git
* curl

## Install

`git clone https://github.com/le0kar0ub1/IWGrAPiC.git && cd IWGrAPiC && make install`

Local installation handled:
  * SDL2
  * SDL2_image
  * SDL2_ttf
  * GrAPiC

## Build & Run

Create a project in IWGrAPiC.

`make project target=$project`

You can code your project in the provided `src` directory (for each project) and put your personnal headers in the `inc` one.

then the following command to build:

`make build target=$project`

and the following one to run;

`make run target=$project` or `./project.bin`

clean the *all* build:

`make clean`

### Epilogue

Feel free to fork, use, improve.
