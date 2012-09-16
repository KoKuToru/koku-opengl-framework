#include "window.h"

/*
	Simple test programm
	
	compile with: g++ *.cpp -lSDL
*/

class test_window: public koku::opengl::windowCallback
{
	private:
		koku::opengl::window my_window;
		bool run;
		
	protected:
		void onQuit()
		{
			//stop everything !
			run = false;
		}

	public:
		test_window(): my_window(this, 640, 480, true), run(true)
		{
			
		}
		
		bool update()
		{
			my_window.update();
			return run;
		}
};

int main()
{
	test_window test;
	
	while(test.update());
	
	return 0;
}

