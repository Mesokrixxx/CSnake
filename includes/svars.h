#ifndef SNAKE_VARS_H
# define SNAKE_VARS_H

# include <stdio.h>
# include <stdlib.h>
# include <SDL2/SDL.h>

// TYPES
# define u32 unsigned int
# define u8 unsigned char

// VARIABLES
# define WindowWidth 1280
# define WindowHeight 720
# define MapWidth 384
# define MapHeight 216

// MACROS
typedef enum { false, true } bool;
typedef struct { float x, y; } vec2;

// Debug
# define ASSERT(_c, ...) if (!(_c)) { fprintf(stderr, __VA_ARGS__); exit(1); }

// MAIN STRUCT
typedef struct snake_s {
	SDL_Window* window;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	u32 pixels[MapWidth * MapHeight];

	bool quit;
} Snake;

#endif