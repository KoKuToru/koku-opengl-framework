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

		class shader_uniform;
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

				int version;

				void checkUniform(shader_uniform* uniform);

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

				void set(shader_uniform *uniform, int item_cout, int count, GLfloat* value);
				void set(shader_uniform* uniform, int item_cout, int count, GLint* value);
				void set(shader_uniform *uniform, int item_cout, int count, GLuint* value);

				void set(shader_uniform* uniform, GLfloat value);
				void set(shader_uniform* uniform, GLint value);
				void set(shader_uniform* uniform, GLuint value);
		};

		class shader_uniform
		{
			friend class shader;

			private:
				std::string name;
				shader* last_sha;
				int last_version;

				GLint id;

			public:
				shader_uniform(std::string name) : name(name), last_sha(nullptr), last_version(-1), id(-1) {}
		};
	}
}

#endif
