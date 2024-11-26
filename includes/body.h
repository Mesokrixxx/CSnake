#ifndef SNAKE_BODY_H
# define SNAKE_BODY_H

#include "svars.h"
#include "graphics.h"

typedef struct body_s {
	int type;
	int drawSize;
	vec2 position, dir;
	u32 headColor, bodyColor;

	struct body_s *next;
} Body;

void bodyInit(int drawSize, u32 headColor, u32 bodyColor);
void bodyUpdate();
void bodyRender();

#endif