//Using SDL and standard IO
#include "SDL.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//Screen dimension constants
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "failed to initialize SDL2.");
		return false;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	SCREEN_WIDTH = displayMode.w;
	SCREEN_HEIGHT = displayMode.h;

	window = SDL_CreateWindow("3D Graphics Programming", 
								   SDL_WINDOWPOS_CENTERED, 
								   SDL_WINDOWPOS_CENTERED,
								   SCREEN_WIDTH,
								   SCREEN_HEIGHT,
								   SDL_WINDOW_BORDERLESS);

	if (window == NULL) {
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}	

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void setup(void) {
	color_buffer = (uint32_t *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));
	color_buffer_texture = SDL_CreateTexture(renderer,
											 SDL_PIXELFORMAT_ARGB8888,
											 SDL_TEXTUREACCESS_STREAMING,
											 SCREEN_WIDTH,
											 SCREEN_HEIGHT);
}

void process_input(void) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT: {				// click "x" button of the window
				is_running = false;
				break;
			}
			case SDL_KEYDOWN: {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					is_running = false;
				}
				break;
			}
			default: {
				break;
			}
		}
	}
}

void update(void) {
	// TODO:
}

void draw_grid(void) {
	// Exercise: Draw a background grid that fills the entire window
	// Lines should be rendered at every row/col multiple of 10.

	const int grid_wh = 10;

	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			if (x % grid_wh == 0 || y % grid_wh == 0) {
				color_buffer[y * SCREEN_WIDTH + x] = 0xFF000000;
			}
		}
	}
}

void clear_color_buffer(uint32_t color) {
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			color_buffer[y * SCREEN_WIDTH + x] = color;
		}
	}
}

void render_color_buffer() {
	SDL_UpdateTexture(color_buffer_texture,
					  NULL,
					  color_buffer,
					  (int)(SCREEN_WIDTH * sizeof(uint32_t)));
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void render(void) {
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	draw_grid();
	render_color_buffer();
	clear_color_buffer(0xFFFFFF00);

	SDL_RenderPresent(renderer);
}

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(void) {
	is_running = initialize_window();	

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;
}
