#ifndef KOKUOPENGLWINDOW
#define KOKUOPENGLWINDOW

#define NO_SDL_GLEXT
#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h> //NO_SDL_GLEXT not working ?

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h> //<SDL2/SDL_opengl.h> not working right
#include <GL/glext.h>

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

		class shader;
		class compute;
		class buffer
		{
			friend class shader;
			friend class compute;

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
		class texture;
		class shader
		{
			friend class compute;

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
				void upload(GLenum what, GLuint& where, std::string source);

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

				void bind(buffer* bind_buffe);

				void set(shader_uniform *uniform, int item_count, int count, GLfloat* value);
				void set(shader_uniform* uniform, int item_count, int count, GLint* value);
				void set(shader_uniform *uniform, int item_count, int count, GLuint* value);

				void set(shader_uniform* uniform, GLfloat value);
				void set(shader_uniform* uniform, GLint value);
				void set(shader_uniform* uniform, GLuint value);

				void set(shader_uniform* uniform, texture* value);
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

		class compute
		{
			private:
				shader my_shader;

			public:
				compute(window* win);

				void upload(std::string source);
				void compile();

				void begin();
				void end();

				void bind(buffer* bind_buffe);

				void set(shader_uniform *uniform, int item_count, int count, GLfloat* value);
				void set(shader_uniform* uniform, int item_count, int count, GLint* value);
				void set(shader_uniform *uniform, int item_count, int count, GLuint* value);

				void set(shader_uniform* uniform, GLfloat value);
				void set(shader_uniform* uniform, GLint value);
				void set(shader_uniform* uniform, GLuint value);

				void set(shader_uniform* uniform, texture* value);

				void execute(int num_groups_x, int num_groups_y, int num_groups_z); //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
		};

		class texture
		{
			friend class shader;

			private:
				window* win;
				GLuint id;
				int w;
				int h;

			public:
				texture(window* win);
				~texture();

				void upload(const char* image, int width, int height, int bytes_per_pixel);
				void upload(const unsigned char* image, int width, int height, int bytes_per_pixel);
				void upload(const float* image, int width, int height, int bytes_per_pixel);
		};
	}
}

#endif
