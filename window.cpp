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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
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

buffer::buffer(window *win):
	win(win), id(0)
{
	win->begin();
	glGenVertexArrays(1, &id);
	//TODO error check
	win->end();
}

buffer::~buffer()
{
	win->begin();
	for(int i = 0; i < uploads.size(); ++i)
	{
		GLuint tmp = uploads[i];
		glDeleteBuffers(1, &tmp);
	}
	uploads.clear();
	glDeleteVertexArrays(1, &id);
	win->end();
}

void buffer::upload(bool element, const float *data, int size, int item_size)
{
	win->begin();
	GLuint tmp;
	glBindVertexArray(id);
	glGenBuffers(1, &tmp);
	glBindBuffer(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, tmp);
	glBufferData(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, sizeof(float)*size, data, GL_STATIC_DRAW);
	if (element == false)
	{
		glEnableVertexAttribArray(uploads.size());
		glVertexAttribPointer(uploads.size(), item_size, GL_FLOAT, GL_FALSE, 0, 0);
	}
	glBindVertexArray(0);
	glBindBuffer(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, 0);
	uploads.push_back(tmp);
	win->end();
}

void buffer::upload(bool element, const unsigned short *data, int size, int item_size)
{
	win->begin();
	GLuint tmp;
	glBindVertexArray(id);
	glGenBuffers(1, &tmp);
	glBindBuffer(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, tmp);
	glBufferData(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, sizeof(unsigned short)*size, data, GL_STATIC_DRAW);
	if (element == false)
	{
		glEnableVertexAttribArray(uploads.size());
		glVertexAttribPointer(uploads.size(), item_size, GL_SHORT, GL_FALSE, 0, 0);
	}
	glBindVertexArray(0);
	glBindBuffer(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, 0);
	uploads.push_back(tmp);
	win->end();
}

void buffer::render(int vertex_per_face, int count)
{
	win->begin();
	glBindVertexArray(id);
	//index should be short... lets hope for right usage..
	switch (vertex_per_face)
	{
		case 1:
			glDrawElements(GL_POINTS, count, GL_UNSIGNED_SHORT, 0);
			break;
		case 2:
			glDrawElements(GL_LINES, count, GL_UNSIGNED_SHORT, 0);
			break;
		case 3:
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);
			break;
		default:
			glPatchParameteri(GL_PATCH_VERTICES, vertex_per_face);
			glDrawElements(GL_PATCHES, count, GL_UNSIGNED_SHORT, 0);
	}
	glBindVertexArray(0);
	win->end();
}

shader::shader(window* win):
	win(win), programm(-1), vertex(-1), tessellation_control(-1), tessellation_eval(-1), geometry(-1), fragment(-1)
{}

shader::~shader()
{
	win->begin();
	if (programm != -1)
	{
		glDeleteProgram(programm);
		programm = -1;
	}
	if (vertex != -1)
	{
		glDeleteShader(vertex);
		vertex = -1;
	}
	if (tessellation_control != -1)
	{
		glDeleteShader(tessellation_control);
		tessellation_control = -1;
	}
	if (tessellation_eval != -1)
	{
		glDeleteShader(tessellation_eval);
		tessellation_eval = -1;
	}
	if (geometry != -1)
	{
		glDeleteShader(geometry);
		geometry = -1;
	}
	if (fragment != -1)
	{
		glDeleteShader(fragment);
		fragment = -1;
	}
	win->end();
}

void shader::uploadVertex(std::string source)
{
	win->begin();
	if (vertex != -1)
	{
		glDeleteShader(vertex);
		vertex = -1;
	}
	vertex = glCreateShader(GL_VERTEX_SHADER);
	const char* source_cstr = source.c_str();
	glShaderSource(vertex, 1, &source_cstr, 0);
	glCompileShader(vertex);
	win->end();
}

void shader::uploadTessellationControl(std::string source)
{
	win->begin();
	if (tessellation_control != -1)
	{
		glDeleteShader(tessellation_control);
		tessellation_control = -1;
	}
	tessellation_control = glCreateShader(GL_TESS_CONTROL_SHADER);
	const char* source_cstr = source.c_str();
	glShaderSource(tessellation_control, 1, &source_cstr, 0);
	glCompileShader(tessellation_control);
	win->end();
}

void shader::uploadTessellationEval(std::string source)
{
	win->begin();
	if (tessellation_eval != -1)
	{
		glDeleteShader(tessellation_eval);
		tessellation_eval = -1;
	}
	tessellation_eval = glCreateShader(GL_TESS_EVALUATION_SHADER);
	const char* source_cstr = source.c_str();
	glShaderSource(tessellation_eval, 1, &source_cstr, 0);
	glCompileShader(tessellation_eval);
	win->end();
}

void shader::uploadGeometry(std::string source)
{
	win->begin();
	if (geometry != -1)
	{
		glDeleteShader(geometry);
		geometry = -1;
	}
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	const char* source_cstr = source.c_str();
	glShaderSource(geometry, 1, &source_cstr, 0);
	glCompileShader(geometry);
	win->end();
}

void shader::uploadFragment(std::string source)
{
	win->begin();
	if (fragment != -1)
	{
		glDeleteShader(fragment);
		fragment = -1;
	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	const char* source_cstr = source.c_str();
	glShaderSource(fragment, 1, &source_cstr, 0);
	glCompileShader(fragment);
	win->end();
}

void printProgramInfoLog(GLuint obj)
{
	if (obj == -1) return;
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		std::cout << infoLog << std::endl;
		free(infoLog);
	}
}

void shader::compile()
{
	win->begin();
	if (programm != -1)
	{
		glDeleteProgram(programm);
		programm = -1;
	}
	programm = glCreateProgram();
	if (vertex != -1)
	{
		glAttachShader(programm, vertex);
	}
	if (tessellation_control != -1)
	{
		glAttachShader(programm, tessellation_control);
	}
	if (tessellation_eval != -1)
	{
		glAttachShader(programm, tessellation_eval);
	}
	if (geometry != -1)
	{
		glAttachShader(programm, geometry);
	}
	if (fragment != -1)
	{
		glAttachShader(programm, fragment);
	}
	glLinkProgram(programm);

	printProgramInfoLog(programm);

	win->end();
}

void shader::begin()
{
	win->begin();
	glUseProgram(programm);
}

void shader::end()
{
	glUseProgram(0);
	win->end();
}
