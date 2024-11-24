#include "graphics.h"

extern Snake game;

void drawLine(int x0, int x1, int y, u32 Color) {
	for (int x = x0; x < x1; x++)
		game.pixels[(y * MapWidth) + x] = Color;
}

void drawSquare(int x0, int x1, int y0, int y1, u32 Color) {
	for (int y = y0; y < y1; y++)
		drawLine(x0, x1, y, Color);
}
