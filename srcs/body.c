#include "body.h"

Body *body;

void bodyInit(int drawSize, u32 headColor, u32 bodyColor) {
	body = calloc(1, sizeof(struct body_s));
	ASSERT(body, "Failed to allocate ram for the snake body\n");

	body->type = 1;
	body->drawSize = drawSize;

	body->position.x = MapWidth / 2;
	body->position.y = MapHeight / 2;
	body->dir.x = 1;
	body->dir.y = 0;

	body->headColor = headColor;
	body->bodyColor = bodyColor;

	body->next = NULL;
}

void bodyUpdate() {
	static int time;
	int bdrawSize = body->drawSize;
	vec2 bdir = body->dir;

	if (time > 50) {
		time = 0;

		if (body->type) {
			body->position.x += bdrawSize * bdir.x;
			body->position.y += bdrawSize * bdir.y;
		}
	}
	else
		time += 1;
}

void bodyRender() {
	vec2 pos = body->position;
	int drawSize = body->drawSize / 2;
	int type = body->type;
	u32 hColor = body->headColor, bColor = body->bodyColor;

	drawSquare(pos.x - drawSize, pos.x + drawSize, pos.y - drawSize, pos.y + drawSize, (type ? bColor : hColor));
}
