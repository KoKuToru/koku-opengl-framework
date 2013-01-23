#ifndef KOKUOPENGLTEXTURE
#define KOKUOPENGLTEXTURE

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
		class rendertarget;
		class texture
		{
			friend class shader;
			friend class rendertarget;

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
