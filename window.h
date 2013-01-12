#ifndef KOKUOPENGLWINDOW
#define KOKUOPENGLWINDOW

#include <SDL2/SDL.h>
#include <string>

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

			public:
				window(windowCallback *callback, std::string title, int width=640, int height=480, bool vsync=true, bool resizeable=false);
				~window();

				void update();
				void flip(int ms_wait=0);
		};
	}
}

#endif
