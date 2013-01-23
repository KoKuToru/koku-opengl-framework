#ifndef KOKUOPENGLSHADER
#define KOKUOPENGLSHADER

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
		class window;
		class shader_uniform;
		class texture;
		class buffer;
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
	}
}

#endif
