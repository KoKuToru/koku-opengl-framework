#ifndef KOKUOPENGLCOMPUTE
#define KOKUOPENGLCOMPUTE

#define NO_SDL_GLEXT
#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h> //NO_SDL_GLEXT not working ?

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h> //<SDL2/SDL_opengl.h> not working right
#include <GL/glext.h>

#include "shader.h"

#include <string>
#include <vector>

namespace koku
{
	namespace opengl
	{
		class window;
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
	}
}

#endif
