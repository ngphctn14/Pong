#include <SDL2/SDL.h>
#include <iostream>

constexpr int SCREEN_WIDTH = 900;
constexpr int SCREEN_HEIGHT = 900;

constexpr float FPS = 60.0f;
constexpr float FRAME_TIME = 1000.0f / FPS;

// forward declarations of structs
struct ball;
struct paddle;
struct render_window;

enum type {
	PLAYER_ONE,
	PLAYER_TWO,
	NUM_OF_PLAYERS
};

struct ball {
	float x, y;
	int r;

	ball(float _x, float _y, int _r) {
		x = _x;
		y = _y;
		r = _r;
	}
};

struct paddle {
	float x, y;
	int w, h;
	short type;
	
	paddle(float _x, float _y, int _w, int _h, short _type) {
		x = _x;
		y = _y;
		w = _w;
		h = _h;
		type = _type;
	}
};

struct render_window {
	SDL_Window* window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	void display() {
		SDL_RenderPresent(renderer);
	}

	void clear() {
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}

	void draw_ball(ball& ball) {

	}

	void draw_paddle(paddle& paddle) {
		SDL_Rect paddle_rect;
		paddle_rect.x = paddle.x;
		paddle_rect.y = paddle.y;
		paddle_rect.w = paddle.w;
		paddle_rect.h = paddle.h;
		SDL_RenderFillRect(renderer, &paddle_rect);
	}

	void draw_middle_line() {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		constexpr int dist = 40;
		for (int i = 0; i < SCREEN_HEIGHT; i += dist) {
			SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2, i, SCREEN_WIDTH/2, i + dist/2);
	}
	}

	~render_window() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
};

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << SDL_GetError() << '\n';
	}

	render_window rw;
	ball ball(200.0f, 200.0f, 20);
	paddle paddles[2] = {paddle(50.0f, 100.0f, 20, 170, PLAYER_ONE), 
						 paddle(SCREEN_WIDTH - 70, 100.0f, 20, 170, PLAYER_TWO)};

	bool quit = false;
	SDL_Event e;

	Uint32 current_time = 0, delta_time = 0;

	while (!quit) {
		current_time = SDL_GetTicks();
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
		rw.clear();
		rw.draw_middle_line();
		for (auto& paddle : paddles) {
			switch(paddle.type) {
				case PLAYER_ONE:
					SDL_SetRenderDrawColor(rw.renderer, 255, 0, 0, 255);
					rw.draw_paddle(paddle);
					break;
				case PLAYER_TWO:
					SDL_SetRenderDrawColor(rw.renderer, 0, 0, 255, 255);
					rw.draw_paddle(paddle);
					break;
				default:
					break;
			}
			SDL_SetRenderDrawColor(rw.renderer, 0, 0, 0, 255);
		}
		rw.display();
		delta_time = SDL_GetTicks() - current_time;
		if (delta_time < FRAME_TIME) {
			SDL_Delay(FRAME_TIME - delta_time);
		}
	}

	SDL_Quit();
	
	return 0;
}
