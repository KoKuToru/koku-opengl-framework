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
	}
}

#endif
