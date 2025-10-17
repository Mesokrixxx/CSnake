#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define MAP_WIDTH 60
#define MAP_HEIGHT 34

#define ASSERT(_expr, _log)				\
	do {								\
		const char *__log = _log;		\
		if (!(_expr)) {					\
			fprintf(stderr,				\
				"[ASSERTION FAILED]"	\
				"[%s:%d][%s]\n%s%s",	\
				__FILE__, __LINE__,		\
				#_expr,					\
				__log ? __log : "",		\
				__log ? "\n" : "");		\
			exit(1);					\
		}								\
	} while (0)

typedef union {
	struct {
		int x;
		int y;
	};
	int raw[2];
}	t_vec2i;

static inline t_vec2i	vec2i(int x, int y) { return (t_vec2i){{ x, y }}; }
static inline t_vec2i	vec2i_adds(t_vec2i v1, t_vec2i v2) { return (vec2i(v1.x + v2.x, v1.y + v2.y)); }
static inline bool		vec2i_eq(t_vec2i v1, t_vec2i v2) { return (v1.x == v2.x && v1.y == v2.y); }
static inline bool		vec2is_contains(t_vec2i *vecs, size_t n, t_vec2i v) {
	for (size_t i = 0; i < n; i++)
		if (vec2i_eq(vecs[i], v))
			return true;
	return false;
}

typedef struct {
	t_vec2i	dir, dirNext;
	t_vec2i	*body;
	t_vec2i headpos;
	size_t	bufcap, bufsize;
}	t_snake;

typedef struct {
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	uint32_t		pixels[MAP_WIDTH * MAP_HEIGHT];
	t_snake			snake;
	t_vec2i			food;
	double			tps;
	bool			running;
}	t_state;

static inline void	pixel_set(uint32_t *pixels, t_vec2i pos, uint32_t col) {
	pixels[(pos.y * MAP_WIDTH) + pos.x] = col;
}

double	time_now(void) {
	static uint64_t freq;
	static uint64_t	start;
	uint64_t		now;
	uint64_t		diff;

	if (!freq)
		freq = SDL_GetPerformanceFrequency();
	now = SDL_GetPerformanceCounter();
	if (!start)
		start = now;
	diff = now - start;
	return ((1000.0 * diff) / freq);
}

void	snake_push(t_snake *snake, t_vec2i pos) {
	if (snake->bufsize >= snake->bufcap) {
		snake->bufcap *= 2;
		snake->body = realloc(snake->body, sizeof(t_vec2i) * snake->bufcap);
		ASSERT(snake->body, NULL);
	}
	memmove(
		snake->body + 1,
		snake->body, sizeof(t_vec2i) * snake->bufsize);
	snake->body[0] = pos;
	snake->headpos = pos;
	snake->bufsize++;
}

void	input_process(t_state *state) {
	const uint8_t *keys = SDL_GetKeyboardState(NULL);
	
	if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_S]) {
		if (!state->snake.dir.y) {
			state->snake.dirNext = 
				vec2i(
					0,
					keys[SDL_SCANCODE_W] ? 1 : -1);
		}
	}
	if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_A]) {
		if (!state->snake.dir.x) {
			state->snake.dirNext = 
				vec2i(
					keys[SDL_SCANCODE_D] ? 1 : -1,
					0);
		}
	}
}

void	food_spawn(t_state *state) {
	state->food = 
		vec2i(
			MAP_WIDTH * ((double)rand() / RAND_MAX),
			MAP_HEIGHT * ((double)rand() / RAND_MAX));
	while (vec2is_contains(state->snake.body, state->snake.bufsize, state->food)) {
		state->food = 
			vec2i(
				MAP_WIDTH * ((double)rand() / RAND_MAX),
				MAP_HEIGHT * ((double)rand() / RAND_MAX));
	}
	pixel_set(state->pixels, state->food, 0xFF0000FF);
}

int	main(void) {
	t_state state = {0};

	ASSERT(!SDL_Init(SDL_INIT_VIDEO), SDL_GetError());

	state.window =
		SDL_CreateWindow(
			"CSnake", 
			SDL_WINDOWPOS_CENTERED_DISPLAY(1), 
			SDL_WINDOWPOS_CENTERED_DISPLAY(1), 
			1080, 720, 
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	ASSERT(state.window, SDL_GetError());

	state.renderer = 
		SDL_CreateRenderer(
			state.window,
			-1, SDL_RENDERER_ACCELERATED);
	ASSERT(state.renderer, SDL_GetError());

	state.texture = 
		SDL_CreateTexture(
			state.renderer,
			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
			MAP_WIDTH, MAP_HEIGHT);
	ASSERT(state.texture, SDL_GetError());

	state.tps = 5.0;

	state.snake.bufcap = 32;
	state.snake.body = malloc(sizeof(t_vec2i) * state.snake.bufcap);
	ASSERT(state.snake.body, NULL);
	state.snake.dir = vec2i(1, 0);
	state.snake.dirNext = state.snake.dir;
	state.snake.bufsize = 1;
	state.snake.body[0] = vec2i(MAP_WIDTH / 2, MAP_HEIGHT / 2);
	state.snake.headpos = state.snake.body[0];

	srand(time(NULL));

	food_spawn(&state);

	double frameLast = 0;
	double elapsed = 0;
	double frameCurr;
	
	state.running = true;
	while (state.running) {
		frameCurr = time_now();
		elapsed += frameCurr - frameLast;
		frameLast = frameCurr;

		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				case (SDL_QUIT):
					state.running = false;
					break ;
				default:
					break ;
			}
		}
		input_process(&state);

		if (frameCurr && elapsed < (1000.0 / state.tps))
			continue ;
		elapsed = 0;

		t_vec2i nextPos = vec2i_adds(state.snake.headpos, state.snake.dirNext);
		if (vec2is_contains(state.snake.body, state.snake.bufsize, nextPos)) {
			printf("Snake collided with itself\n");
			state.running = false;
			break ;
		} else if (!(nextPos.x >= 0 && nextPos.x < MAP_WIDTH)
				|| !(nextPos.y >= 0 && nextPos.y < MAP_HEIGHT)) {
			printf("Snake out of bounds\n");
			state.running = false;
			break ;
		} else if (vec2i_eq(nextPos, state.food)) {
			food_spawn(&state);
			snake_push(&state.snake, nextPos);
			pixel_set(state.pixels, state.snake.headpos, 0x00FF00FF);
			state.tps += 0.33;
		} else {
			snake_push(&state.snake, nextPos);
			pixel_set(state.pixels, nextPos, 0x00FF00FF);
			pixel_set(state.pixels, state.snake.body[state.snake.bufsize - 1], 0);
			state.snake.bufsize--;
		}
		state.snake.dir = state.snake.dirNext;

		SDL_UpdateTexture(state.texture, NULL, state.pixels, MAP_WIDTH * 4);
		SDL_RenderCopyEx(
			state.renderer, 
			state.texture, 
			NULL, NULL, 
			0, NULL, 
			SDL_FLIP_VERTICAL);
		SDL_RenderPresent(state.renderer);
	}

	free(state.snake.body);
	SDL_DestroyTexture(state.texture);
	SDL_DestroyRenderer(state.renderer);
	SDL_DestroyWindow(state.window);
	SDL_Quit();
	return 0;
}
