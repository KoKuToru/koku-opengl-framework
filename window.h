#ifndef KOKUOPENGLWINDOW
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

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
				windowCallback *callback;
				SDL_Surface *context;		
				static bool first_time;
				
			public:
				window(windowCallback *callback, int width=640, int height=480, bool vsync=true);
				~window();
				
				void update();
		};
	}
}

#endif
