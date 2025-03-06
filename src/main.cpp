#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <iostream>

#define WIDTH 900
#define HEIGHT 600

#define WHITE 0xffffffff
#define BLACK 0x00000000

void ClearScreen(SDL_Surface& surface, SDL_Rect& full)
{
	SDL_FillRect(&surface, &full, BLACK);
}

int Clamp(int& i, int min, int max)
{
	if (i < min) return min;
	if (i > max) return max;
	return i;
}

int main()
{
	bool running = true;
	bool drawing = false;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("SDL Paint", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_Surface* surface = SDL_GetWindowSurface(window);

	SDL_Rect full;
	SDL_Rect rect;
	full.x = 0;
	full.y = 0;
	full.w = WIDTH;
	full.h = HEIGHT;

	int currentColor = 0xffffffff;

	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) running = false;
			if (event.type == SDL_MOUSEBUTTONDOWN) drawing = true;
			if (event.type == SDL_MOUSEBUTTONUP) drawing = false;
			if (event.type == SDL_MOUSEWHEEL)
			{
				std::cout << currentColor << std::endl;
				currentColor += event.wheel.preciseY;
				//currentColor = Clamp(currentColor, BLACK, WHITE);
			}
		}



		int min = (int)BLACK;
		int max = (int)WHITE;
		int currentColor = WHITE;

		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		rect.x = mouseX;
		rect.y = mouseY;
		rect.w = 2;
		rect.h = 2;
		if (drawing) SDL_FillRect(surface, &rect, currentColor);

		SDL_UpdateWindowSurface(window);
	}

	return 0;
}