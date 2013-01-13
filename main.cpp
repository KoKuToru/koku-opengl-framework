#include "window.h"

/*
	Simple test programm

	compile with: g++ *.cpp -lSDL
*/

class test_window: public koku::opengl::windowCallback
{
	private:
		koku::opengl::window my_window;
		koku::opengl::buffer my_buffer;
		koku::opengl::shader my_shader;
		bool run;

	protected:
		void onQuit()
		{
			//stop everything !
			run = false;
		}

	public:
		test_window(): my_window(this, "test", 640, 480, true), my_buffer(&my_window), my_shader(&my_window), run(true)
		{
			const float vertex[] =
			{
				1*0.8,1*0.8,0, //right top
				-1*0.8,1*0.8,0, //left top
				-1*0.8,-1*0.8,0, //left bottom
				1*0.8, -1*0.8, 0 //right bottom
			};

			const float color[] =
			{
				0,0,0,
				1,0,0,
				0,1,0,
				0,0,1
			};

			const unsigned short index[] =
			{
				0,1,3,2
			};

			my_buffer.upload(false, vertex, 4*3, 3);
			my_buffer.upload(false,  color, 4*3, 3);
			my_buffer.upload(true,   index, 4*1, 1);

			my_shader.uploadVertex("#version 400\n"
								   "layout(location = 0) in vec3 Position;\n"
								   "layout(location = 1) in vec3 Color;\n"
								   "out vec3 Position_tc;\n"
								   "void main()\n"
								   "{\n"
								   "	Position_tc = Position;\n" //pass position to tesslelation control shader
								   "}\n");

			my_shader.uploadTessellationControl("#version 400\n"
												"layout(vertices = 4) out;"
												"in vec3 Position_tc[];\n"
												"out vec3 Position_te[];\n"
												"#define ID gl_InvocationID\n"
												"void main()\n"
												"{\n"
												"	Position_te[ID] = Position_tc[ID];\n"
												"	if (ID != 0)\n"
												"	{\n"
												"		gl_TessLevelInner[0] = 16;\n" //tes-level
												"		gl_TessLevelInner[0] = int(int(gl_TessLevelInner[0])/2)*2;\n" //only 1,2,4,6, ...
												"		gl_TessLevelInner[1] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[0] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[1] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[2] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[3] = gl_TessLevelInner[0];\n"
												"	}\n"
												"}\n");

			my_shader.uploadTessellationEval("#version 400\n"
											 "layout(quads, equal_spacing) in;\n"
											 "in vec3 Position_te[];\n"
											 "void main()\n"
											 "{\n"
											 "	float u = gl_TessCoord.x, v = gl_TessCoord.y;\n"
											 "	vec3 a = mix(Position_te[0], Position_te[1], u);\n"
											 "	vec3 b = mix(Position_te[2], Position_te[3], u);\n"
											 "	gl_Position = vec4(mix(a, b, v), 1.0);\n"
											 "}\n");

			my_shader.uploadFragment("#version 400\n"
									 "layout(location = 0) out vec4 FragColor;\n"
									 "void main()\n"
									 "{\n"
									 "	FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
									 "}\n");

			my_shader.compile();
		}

		bool update()
		{
			my_window.update();

			my_window.begin();
				glClearColor(0,1,0,1);
				glClear(GL_COLOR_BUFFER_BIT);
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //uhm no lights yet
			my_window.end();

			my_shader.begin();
				my_buffer.render(4, 4);
			my_shader.end();

			my_window.flip(1000/30); //max. 30Hz
			return run;
		}
};

int main()
{
	test_window test;

	while(test.update());

	return 0;
}

