#include "window.h"

using namespace koku;
using namespace opengl;

int window::sdl_count = 0;

window::window(windowCallback *callback, std::string title, int width, int height, bool vsync, bool resizeable):
	callback(callback), win(nullptr), win_w(width), win_h(height), ren(nullptr)
{
	if (sdl_count == 0)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		{
			throw std::string(SDL_GetError());
		}
	}
	sdl_count += 1;

	win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_w, win_h, SDL_WINDOW_SHOWN | (resizeable?SDL_WINDOW_RESIZABLE:0));
	if (win == nullptr)
	{
		throw std::string(SDL_GetError());
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | (vsync?SDL_RENDERER_PRESENTVSYNC:0));
	if (ren == nullptr)
	{
		throw std::string(SDL_GetError());
	}
}

window::~window()
{
	if (ren != nullptr)
	{
		SDL_DestroyRenderer(ren);
		ren = nullptr;
	}

	if (win != nullptr)
	{
		SDL_DestroyWindow(win);
		win = nullptr;
	}

	if (--sdl_count == 0)
	{
		//last sdl window
		SDL_Quit();
	}
}

void window::update()
{
	//handle events
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_QUIT:
				//or not ? see SQL_WINDOWEVENT. SDL_WINDOWEVENT_CLOSE
				callback->onQuit();
				break;
			case SDL_WINDOWEVENT:
				if (e.window.windowID == SDL_GetWindowID(win))
				{
					switch (e.window.event)
					{
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							win_w = e.window.data1;
							win_h = e.window.data2;
							callback->onResize(win_w, win_h);
							break;
						case SDL_WINDOWEVENT_CLOSE:
							callback->onQuit();
							break;
						default:
							break;
					}
				}
				else
				{
					//TODO .. handle multiply windows ??
					//what should I do ? now ?
				}
				break;
			default:
				break;
		}
	}
}

void window::flip(int ms_wait)
{
	SDL_RenderPresent(ren);
	if (ms_wait > 0)
	{
		SDL_Delay(ms_wait);
	}
}
