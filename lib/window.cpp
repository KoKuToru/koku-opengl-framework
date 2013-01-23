#include "window.h"
#include <iostream>
#include <signal.h>

using namespace koku;
using namespace opengl;

int window::sdl_count = 0;

void debugOutput(unsigned int source, unsigned int type, unsigned int id,
				 unsigned int severity, int length,
				 const char* message, void* userParam)
{
	if (std::string(message, std::string("Buffer detailed info").size()) == "Buffer detailed info")
	{
		//filter it
		return;
	}

	std::string error_source, error_type, error_severity;
	switch(source)
	{
		case GL_DEBUG_SOURCE_API_ARB:
			error_source = "OpenGL";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
			error_source = "Windows";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
			error_source = "Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
			error_source = "Third Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION_ARB:
			error_source = "Application";
			break;
		case GL_DEBUG_SOURCE_OTHER_ARB:
			error_source = "Other";
			break;
		default:
			error_source = "UNKNOW";
			break;
	}

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR_ARB:
			error_type = "Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
			error_type = "Deprecated behavior";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
			error_type = "Undefined behavior";
			break;
		case GL_DEBUG_TYPE_PORTABILITY_ARB:
			error_type = "Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE_ARB:
			error_type = "Performance";
			break;
		case GL_DEBUG_TYPE_OTHER_ARB:
			error_type = "Other";
		default:
			error_type = "UNKNOW";
			break;
	}

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH_ARB:
			error_severity = "High";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM_ARB:
			error_severity = "Medium";
			break;
		case GL_DEBUG_SEVERITY_LOW_ARB:
			error_severity = "Low";
			break;
		default:
			error_severity = "UNKNOW";
			break;
	}

	std::cout << "Source: " << error_source << "  Type: " << error_type << "  Severity: " << error_severity << "  Message: " << message << std::endl;

	if (severity == GL_DEBUG_SEVERITY_HIGH_ARB)
	{
		raise(SIGTRAP);
	}
}

window::window(windowCallback *callback, std::string title, int width, int height, bool vsync, bool resizeable):
	callback(callback), win(nullptr), win_w(width), win_h(height), ren(nullptr), ctx(nullptr)
{
	if (sdl_count == 0)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		{
			throw std::string(SDL_GetError());
		}
	}
	sdl_count += 1;

	win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_w, win_h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (resizeable?SDL_WINDOW_RESIZABLE:0));
	if (win == nullptr)
	{
		throw std::string(SDL_GetError());
	}

	/*
	 *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | (vsync?SDL_RENDERER_PRESENTVSYNC:0));
	if (ren == nullptr)
	{
		throw std::string(SDL_GetError());
	}
	*/

	//OpenGL 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	ctx = SDL_GL_CreateContext(win);
	if (ctx == nullptr)
	{
		throw std::string(SDL_GetError());
	}

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageCallbackARB(debugOutput, NULL);
}

window::~window()
{
	if (ctx != nullptr)
	{
		SDL_GL_DeleteContext(ctx);
		ctx = nullptr;
	}

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
	//SDL_RenderPresent(ren);
	SDL_GL_SwapWindow(win);
	if (ms_wait > 0)
	{
		SDL_Delay(ms_wait);
	}
}

void window::begin()
{
	SDL_GL_MakeCurrent(win, ctx);
}

void window::end()
{
	//nothing atm
}
