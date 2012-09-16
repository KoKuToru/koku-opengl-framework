#include "window.h"

using namespace koku;
using namespace opengl;

bool window::first_time = true;

window::window(windowCallback *callback, int width, int height, bool vsync): callback(callback)
{
	if (first_time)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			throw /*TODO*/"error";
		}
		
		first_time = false;
	}
	else
	{
		throw /*TODO*/"error - no multiply windows possible ! SDL 1.2 limitation";
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	context = SDL_SetVideoMode(width, height, 0, SDL_OPENGL | ((callback==0)?0:SDL_RESIZABLE));
	
	if (!context)
	{
		throw /*TODO*/"error";
	}
	
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, vsync?1:0);

}

window::~window()
{
	if (context)
	{
		SDL_FreeSurface(context);
	}

	if (!first_time)
	{
		SDL_Quit();
		first_time = true;
	}
}

void window::update()
{	
	SDL_Event event;
	
	if (callback == 0)
	{
		//kill all events..		
		while(SDL_PollEvent(&event) != 0);
		return;
	}
	
	while (SDL_PollEvent(&event) != 0)
	{
		//there is something todo !
		switch (event.type)
		{
			case SDL_VIDEORESIZE:
				callback->onResize(event.resize.w, event.resize.h);
				break;
			case SDL_QUIT:
				callback->onQuit();
				break;
		}
	}
}
