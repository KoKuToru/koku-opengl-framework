#include "window.h"
#include <cmath>

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
		koku::opengl::shader_uniform my_camera_pos;

		bool run;

	protected:
		void onQuit()
		{
			//stop everything !
			run = false;
		}

	public:
		test_window(): my_window(this, "test", 640, 480, true), my_buffer(&my_window), my_shader(&my_window), my_camera_pos("camera_pos"), run(true)
		{
			const float vertex[] =
			{
				1*0.8,1*0.8,-1, //right top
				-1*0.8,1*0.8,-1, //left top
				-1*0.8,-1*0.8,-1, //left bottom
				1*0.8, -1*0.8, -1, //right bottom

				1*0.8,1*0.8,1, //right top
				-1*0.8,1*0.8,1, //left top
				-1*0.8,-1*0.8,1, //left bottom
				1*0.8, -1*0.8, 1 //right bottom
			};

			const float color[] =
			{
				1,0,0,
				1,0,0,
				1,0,0,
				1,0,0,

				0,1,0,
				0,1,0,
				0,1,0,
				0,1,0
			};

			const unsigned short index[] =
			{
				0,1,3,2,0+4,1+4,3+4,2+4
			};

			my_buffer.upload(false, vertex, 8*3, 3);
			my_buffer.upload(false,  color, 8*3, 3);
			my_buffer.upload(true,   index, 8*1, 1);

			my_shader.uploadVertex("#version 400\n"
								   "layout(location = 0) in vec3 Position;\n"
								   "layout(location = 1) in vec3 Color;\n"
								   "out vec3 Position_tc;\n"
								   "out vec3 Color_tc;\n"
								   "void main()\n"
								   "{\n"
								   "	Position_tc = Position;\n" //pass position to tesslelation control shader
								   "	Color_tc = Color;\n"
								   "}\n");

			my_shader.uploadTessellationControl("#version 400\n"
												"layout(vertices = 4) out;"
												"in vec3 Position_tc[];\n"
												"in vec3 Color_tc[];\n"
												"out vec3 Position_te[];\n"
												"out vec3 Color_te[];\n"
												"out mat4 ModelViewMatrix_te[];\n"
												"uniform vec3 camera_pos;\n"
												"#define ID gl_InvocationID\n"
												"void main()\n"
												"{\n"
												"	Position_te[ID] = Position_tc[ID];\n"
												"	Color_te[ID] = Color_tc[ID];\n"
												"	if (ID != 0)\n"
												"	{\n"
												"		gl_TessLevelInner[0] = 4;\n" //tes-level
												//"		gl_TessLevelInner[0] = int(int(gl_TessLevelInner[0])/2)*2;\n" //only 1,2,4,6, ...
												"		gl_TessLevelInner[1] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[0] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[1] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[2] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[3] = gl_TessLevelInner[0];\n"
												"	}\n"
												/* CALCULATE MODEL VIEW MATRIX WITHIN GLSL (for each face once, stupid but who cares.. hate CPU-Side OpenGL)*/
												"	vec3 from = camera_pos;\n" //from to doesn't work right ? no idea why
												"	vec3 to   = vec3(0,0,0);\n"
												"	vec3 dir  = normalize(to - from);\n"
												"	vec3 up   = vec3(0,1,0);\n"
												"	vec3 right= cross(dir, up);\n"
												"	up = cross(dir, right);\n"
												"	ModelViewMatrix_te[ID] = mat4(vec4(right, 0),\n"
												"								  vec4(up   , 0),\n"
												"								  vec4(dir  , 0),\n"
												"								  vec4(-from, 1));\n"
												"	ModelViewMatrix_te[ID] = inverse(ModelViewMatrix_te[ID]);\n"
												"}\n");

			my_shader.uploadTessellationEval("#version 400\n"
											 "layout(quads, equal_spacing) in;\n"
											 "in vec3 Position_te[];\n"
											 "in vec3 Color_te[];\n"
											 "in mat4 ModelViewMatrix_te[];\n"
											 "out vec3 Color_geo;\n"
											 "out vec3 Position_geo;\n"
											 "out mat4 ModelViewMatrix_geo;\n"
											 "void main()\n"
											 "{\n"
											 "	float u = gl_TessCoord.x, v = gl_TessCoord.y;\n"
											 "	vec3 a = mix(Position_te[0], Position_te[1], u);\n"
											 "	vec3 b = mix(Position_te[2], Position_te[3], u);\n"
											 "	Position_geo = mix(a, b, v);\n" //pass to geometry shader
											 "	a = mix(Color_te[0], Color_te[1], u);\n"
											 "	b = mix(Color_te[2], Color_te[3], u);\n"
											 "	Color_geo = mix(a, b, v);\n"
											 "	ModelViewMatrix_geo = ModelViewMatrix_te[0];\n" //doesn't matter which we copy (should be all the same)
											 "}\n");

			my_shader.uploadGeometry("#version 400\n"
									 //"layout(quads) in;\n"
									 "layout(triangles) in;\n" //can't use quads
									 "layout(triangle_strip, max_vertices=24) out;\n"
									 "in vec3 Position_geo[];\n"
									 "in vec3 Color_geo[];\n"
									 "in mat4 ModelViewMatrix_geo[];\n"
									 "out vec3 Color_fr;\n"
									 "void main()\n"
									 "{\n"
									 /* PROJECTION */
									 "	float h = 1.0/tan(45*0.0087266);\n" //FOV
									 "	float zNear = 0.1;\n"
									 "	float zFar  = 1000.0; \n"
									 "	float frustrumLength = zFar - zNear;\n"
									 "	mat4 ProjectionMatrix = mat4(vec4(h, 0, 0, 0),\n"
									 "								 vec4(0, h, 0, 0),\n"
									 "								 vec4(0, 0, -(zFar + zNear)/frustrumLength, -1),\n"
									 "								 vec4(0, 0, -2.0*(zNear * zFar)/frustrumLength, 0));\n"
									 /* PROJECTION END */
									 "	for(int i = 0; i < 3; i++)\n"
									 "	{\n"
									 "		gl_Position = ProjectionMatrix * ModelViewMatrix_geo[0] * vec4(Position_geo[i], 1.0);\n" //again doesn't matter which ModelViewMatrix_geo (should be all the same)
									 "		Color_fr = Color_geo[i];\n"
									 "		EmitVertex();\n"
									 "	}\n"
									 "	EndPrimitive();\n"
									 //Dup it
									 "	for(int i = 0; i < 3; i++)\n"
									 "	{\n"
									 "		gl_Position = ProjectionMatrix * ModelViewMatrix_geo[0] * vec4(Position_geo[i] + vec3(0,0,0.25), 1.0);\n" //again doesn't matter which ModelViewMatrix_geo (should be all the same)
									 "		Color_fr = Color_geo[i] + vec3(0,0,1);\n"
									 "		EmitVertex();\n"
									 "	}\n"
									 "	EndPrimitive();\n"
									 //Extrude it ;)
									 /*"	vec4 new[3];\n"
									 "	vec4 old[3];\n"
									 "	old[0] = ProjectionMatrix * ModelViewMatrix_geo[0] * vec4(Position_geo[0],1);\n"
									 "	old[1] = ProjectionMatrix * ModelViewMatrix_geo[0] * vec4(Position_geo[1],1);\n"
									 "	old[2] = ProjectionMatrix * ModelViewMatrix_geo[0] * vec4(Position_geo[2],1);\n"
									 "	new[0] = ProjectionMatrix * ModelViewMatrix_geo[0] * vec4(Position_geo[0] + vec3(0,0,0.5),1);\n"
									 "	new[1] = ProjectionMatrix * ModelViewMatrix_geo[0] * vec4(Position_geo[1] + vec3(0,0,0.5),1);\n"
									 "	new[2] = ProjectionMatrix * ModelViewMatrix_geo[0] * vec4(Position_geo[2] + vec3(0,0,0.5),1);\n"
									 "	Color_fr = vec3(0,0,0);\n"
									 "	gl_Position = old[2]; EmitVertex();\n"
									 "	gl_Position = old[0]; EmitVertex();\n"
									 "	gl_Position = new[0]; EmitVertex();\n"
									 "	EndPrimitive();\n"
									 "	gl_Position = old[2]; EmitVertex();\n"
									 "	gl_Position = new[0]; EmitVertex();\n"
									 "	gl_Position = new[2]; EmitVertex();\n"
									 "	EndPrimitive();\n"
									 "	gl_Position = old[0]; EmitVertex();\n"
									 "	gl_Position = old[1]; EmitVertex();\n"
									 "	gl_Position = new[1]; EmitVertex();\n"
									 "	EndPrimitive();\n"
									 "	gl_Position = old[0]; EmitVertex();\n"
									 "	gl_Position = new[1]; EmitVertex();\n"
									 "	gl_Position = new[0]; EmitVertex();\n"
									 "	EndPrimitive();\n"
									 "	gl_Position = old[1]; EmitVertex();\n"
									 "	gl_Position = old[2]; EmitVertex();\n"
									 "	gl_Position = new[2]; EmitVertex();\n"
									 "	EndPrimitive();\n"
									 "	gl_Position = old[1]; EmitVertex();\n"
									 "	gl_Position = new[2]; EmitVertex();\n"
									 "	gl_Position = new[1]; EmitVertex();\n"
									 "	EndPrimitive();\n"
									 "	gl_Position = new[0]; EmitVertex();\n"
									 "	gl_Position = new[1]; EmitVertex();\n"
									 "	gl_Position = new[2]; EmitVertex();\n"
									 "	EndPrimitive();\n"*/
									 "}\n");

			my_shader.uploadFragment("#version 400\n"
									 "layout(location = 0) out vec4 FragColor;\n"
									 "in vec3 Color_fr;\n"
									 "void main()\n"
									 "{\n"
									 "	FragColor = vec4(Color_fr, 1.0);\n"
									 "}\n");

			my_shader.compile();
		}

		bool update()
		{
			my_window.update();

			my_window.begin();
				glClearColor(1,1,1,1);
				glClear(GL_COLOR_BUFFER_BIT);
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //uhm no lights yet
			my_window.end();

			static int frame = 0;
			frame = frame + 1;

			GLfloat pos[3] = {std::cos(frame/100.0f)*10.0f, 10, std::sin(frame/100.0f)*10.0f};
			my_shader.set(&my_camera_pos, 3, 1, pos); //not that sure if this is syntax is nice

			my_shader.begin();
				my_buffer.render(4, 8);
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

