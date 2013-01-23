#ifndef KOKUOPENGLBUFFER
#define KOKUOPENGLBUFFER

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
	}
}

#endif
