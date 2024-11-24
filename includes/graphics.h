# ifndef SNAKE_GRAPHICS_H
# define SNAKE_GRAPHICS_H

#include "svars.h"

# define BLUE 0xFF0000
# define GREEN 0x00FF00
# define RED 0x0000FF
# define BLACK 0x000000
# define WHITE 0xFFFFFF
# define PURPLE 0x800080
# define YELLOW 0x00FFFF
# define CYAN 0xFFFF00
# define PINK 0xFF00FF

void drawLine(int x0, int x1, int y, u32 Color);
void drawSquare(int x0, int x1, int y0, int y1, u32 Color);

#endif