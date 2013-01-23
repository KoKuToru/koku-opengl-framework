#ifndef KOKUOPENGLRENDERTARGET
#define KOKUOPENGLRENDERTARGET

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
		class texture;
		class rendertarget
		{
			private:
				window* win;
				GLuint rbo_id;
				GLuint fbo_id;
				int w;
				int h;
				std::vector<GLuint> textures;
				GLint v_dim[4];

			public:
				rendertarget(window* win);
				~rendertarget();

				void begin();
				void end();

				void bind(texture* my_tex, int tex_count=1);
		};
	}
}

#endif
