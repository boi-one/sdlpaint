#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

static int WIDTH = 900;
static int HEIGHT = 600;

#define RED     0xffff0000
#define GREEN   0xff00ff00
#define BLUE    0xff0000ff
#define YELLOW  0xffffff00
#define CYAN    0xff00ffff
#define MAGENTA 0xffff00ff
#define WHITE   0xffffffff
#define BLACK   0xff000000

int Clamp(int value, int min, int max)
{
	if (max < min)
	{
		int	temp = min;
		min = max;
		max = temp;
	}
	if (value < min) value = min;
	if (value > max) value = max;
	return value;
}

void ClearScreen(SDL_Surface* s)
{
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = WIDTH;
	r.h = HEIGHT;

	SDL_FillRect(s, &r, BLACK);
}

void SetPixel(SDL_Surface* s, int x, int y, unsigned int c)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = 1;
	r.h = 1;
	SDL_FillRect(s, &r, c);
}

void DrawCircle(SDL_Surface* s, int radius, int mouseX, int mouseY, unsigned int color)
{
	int centerX = mouseX - radius;
	int centerY = mouseY - radius;

	int radiusSqrt = radius * radius;

	for (int x = 0; x < radius * 2; x++) for (int y = 0; y < radius * 2; y++)
	{
		int cx = x - radius;
		int cy = y - radius;

		int centerSqrt = cx * cx + cy * cy;
		if (centerSqrt < radiusSqrt)	SetPixel(s, x + centerX, y + centerY, color);
	}
}

int main()
{
	bool running = true;
	bool drawing = false;
	bool rainbow = false;
	int penSize = 2;
	unsigned int currentColor = WHITE;
	int time = 0;
	unsigned int colors[] =
	{
		RED,
		GREEN,
		BLUE,
		YELLOW,
		MAGENTA,
		CYAN
	};
	int colorIndex = 0;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("SDL Paint", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED));
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_Surface* surface = SDL_GetWindowSurface(window);

	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				drawing = true;
				break;
			case SDL_MOUSEBUTTONUP:
				drawing = false;
				break;
			case SDL_KEYDOWN:
			{
				rainbow = false;
				switch (event.key.keysym.sym)
				{
				case SDLK_1:
					currentColor = WHITE;
					break;
				case SDLK_2:
					currentColor = BLACK;
					break;
				case SDLK_3:
					currentColor = RED;
					break;
				case SDLK_4:
					currentColor = GREEN;
					break;
				case SDLK_5:
					currentColor = BLUE;
					break;
				case SDLK_6:
					currentColor = YELLOW;
					break;
				case SDLK_7:
					currentColor = MAGENTA;
					break;
				case SDLK_8:
					currentColor = CYAN;
					break;
				case SDLK_9:
					rainbow = !rainbow;
					break;
				case SDLK_0:
					ClearScreen(surface);
					break;
				case SDLK_s:
				{
					std::string name = "painting ";

					std::time_t now = std::time(0);
					std::tm* currentTime = std::localtime(&now);
					std::ostringstream oss;
					oss << std::put_time(currentTime, "%d-%m-%Y %H-%M-%S");
					name.append(oss.str());
					name.append(".png");

					std::cout << IMG_SavePNG(surface, name.c_str()) << std::endl;
				}break;
				case SDLK_ESCAPE:
					running = false;
					break;
				}
			}break;
			case SDL_MOUSEWHEEL:
			{
				if (event.wheel.y < 0) penSize--;
				if (event.wheel.y > 0) penSize++;
				penSize = Clamp(penSize, 2, 20);
			}
			break;
			}
		}

		SDL_GetWindowSize(window, &WIDTH, &HEIGHT);

		if (SDL_GetTicks() > time)
		{
			time = SDL_GetTicks() + 100;
			if (rainbow)
			{
				currentColor = colors[colorIndex];
				colorIndex = colorIndex < sizeof(colors) / sizeof(int) ? ++colorIndex : 0;
			}
		}

		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		if (drawing) DrawCircle(surface, penSize, mouseX, mouseY, currentColor);

		SDL_UpdateWindowSurface(window);
	}

	return 0;
}