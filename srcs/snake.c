# include "snake.h"

struct snake_s {
	SDL_Window* window;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	unsigned int pixels[MapWidth * MapHeight];

	bool quit;
} game;

int main() {
	ASSERT(!SDL_Init(SDL_INIT_VIDEO),
		"Failed to init SDL video: %s\n",
		SDL_GetError());

	game.window = SDL_CreateWindow(
					"C SNAKE",
					SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1),
					WindowWidth, WindowHeight,
					SDL_WINDOW_ALLOW_HIGHDPI);
	ASSERT(game.window, "Failed to create SDL window: %s\n", SDL_GetError());

	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_PRESENTVSYNC);
	ASSERT(game.renderer, "Failed to create SDL renderer: %s\n", SDL_GetError());

	game.texture = SDL_CreateTexture(
						game.renderer,
						SDL_PIXELFORMAT_ABGR8888,
						SDL_TEXTUREACCESS_STREAMING,
						MapWidth, MapHeight);
	ASSERT(game.texture, "Failed to create SDL texture: %s\n", SDL_GetError());

	while (!game.quit) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				case SDL_QUIT:
					game.quit = true;
					break ;
			}
		}

		const char *keyState = SDL_GetKeyboardState(NULL);

		if (keyState[SDL_SCANCODE_ESCAPE])
			game.quit = true;

		game.pixels[155 * 25] = PINK;

		SDL_UpdateTexture(game.texture, NULL, game.pixels, MapWidth * 4);
		SDL_RenderCopyEx(
			game.renderer,
			game.texture,
			NULL, NULL,
			0.0,
			NULL,
			SDL_FLIP_VERTICAL);
		SDL_RenderPresent(game.renderer);
	}

	SDL_DestroyTexture(game.texture);
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);

	return (0);
}
