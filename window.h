#ifndef KOKUOPENGLWINDOW
#define KOKUOPENGLWINDOW

#include <SDL2/SDL.h>

#define GL_GLEXT_PROTOTYPES 1

#include <SDL2/SDL_opengl.h>
#include <string>
#include <vector>

namespace koku
{
	namespace opengl
	{
		class windowCallback
		{
			public:
				virtual void onResize(int width, int height) {}
				virtual void onQuit() {}
		};

		class window
		{
			private:
				static int sdl_count;

				windowCallback *callback;
				SDL_Window *win;
				int win_w, win_h;
				SDL_Renderer *ren;
				SDL_GLContext ctx;

			public:
				window(windowCallback *callback, std::string title, int width=640, int height=480, bool vsync=true, bool resizeable=false);
				~window();

				void update();
				void flip(int ms_wait=0);
				//render control:
				void begin();
				void end();
		};

		class buffer
		{
			private:
				window* win;
				GLuint id;

				std::vector<GLuint> uploads;

			public:
				buffer(window* win);
				~buffer();

				void upload(bool element, const float* data, int size, int item_size);
				void upload(bool element, const unsigned short* data, int size, int item_size);
				void render(int vertex_per_face, int count);
		};

		class shader
		{
			private:
				window* win;
				GLuint programm;
				GLuint vertex;
				GLuint tessellation_control;
				GLuint tessellation_eval;
				GLuint geometry;
				GLuint fragment;

			public:
				shader(window* win);
				~shader();
				void uploadVertex(std::string source);
				void uploadTessellationControl(std::string source);
				void uploadTessellationEval(std::string source);
				void uploadGeometry(std::string source);
				void uploadFragment(std::string source);

				void compile();

				void begin();
				void end();
		};
	}
}

#endif
