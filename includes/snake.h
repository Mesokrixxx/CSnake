#ifndef SNAKE_H
# define SNAKE_H

# include <stdio.h>
# include <stdlib.h>
# include <SDL2/SDL.h>

# include "graphics.h"

// VARIABLES
# define WindowWidth 1280
# define WindowHeight 720
# define MapWidth 384
# define MapHeight 216

// Debug
# define ASSERT(_c, ...) if (!(_c)) { fprintf(stderr, __VA_ARGS__); exit(1); }

// Boolean
typedef enum { false, true } bool;

#endif