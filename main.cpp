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
		koku::opengl::compute my_compute;
		koku::opengl::texture my_texture;
		koku::opengl::shader_uniform my_texture_pos;
		bool run;

	protected:
		void onQuit()
		{
			//stop everything !
			run = false;
		}

	public:
		test_window(): my_window(this, "test", 640, 480, true), my_buffer(&my_window), my_shader(&my_window), my_camera_pos("camera_pos"), my_compute(&my_window), my_texture(&my_window), my_texture_pos("texture"), run(true)
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

			const float uv[] =
			{
				0,0,
				1,0,
				1,1,
				0,1,

				0,0,
				1,0,
				1,1,
				0,1
			};

			const float camera_matrix[] =
			{
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};

			const unsigned short index[] =
			{
				0,1,3,2,0+4,1+4,3+4,2+4
			};

			const char texture[] =
			{
				-1,0,0,	-1,0,0,	-1,0,0,	-1,0,0,
				-1,0,0,	0,-1,0,	0,0,0,	-1,0,0,
				-1,0,0,	0,0,0,	0,-1,0,	-1,0,0,
				-1,0,0,	-1,0,0,	-1,0,0,	-1,0,0
			}; //4x4 texture

			my_buffer.upload(false,        vertex, 8*3, 3);
			my_buffer.upload(false,            uv, 8*2, 2);
			my_buffer.upload(false, camera_matrix, 4*4, 1); //gets calculated by compute_shader ;)
			my_buffer.upload(true,          index, 8*1, 1);
			my_texture.upload((const unsigned char*)texture, 4, 4, 3);

			my_shader.uploadVertex("#version 400\n"
								   "layout(location = 0) in vec3 Position;\n"
								   "layout(location = 1) in vec2 UV;\n"
								   "out vec3 Position_tc;\n"
								   "out vec2 UV_tc;\n"
								   "void main()\n"
								   "{\n"
								   "	Position_tc = Position;\n" //pass position to tesslelation control shader
								   "	UV_tc = UV;\n"
								   "}\n");

			my_shader.uploadTessellationControl("#version 400\n"
												"layout(vertices = 4) out;"
												"in vec3 Position_tc[];\n"
												"in vec2 UV_tc[];\n"
												"out vec3 Position_te[];\n"
												"out vec2 UV_te[];\n"
												"#define ID gl_InvocationID\n"
												"void main()\n"
												"{\n"
												"	Position_te[ID] = Position_tc[ID];\n"
												"	UV_te[ID] = UV_tc[ID];\n"
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
												"}\n");

			my_shader.uploadTessellationEval("#version 400\n"
											 "layout(quads, equal_spacing) in;\n"
											 "in vec3 Position_te[];\n"
											 "in vec2 UV_te[];\n"
											 "out vec2 UV_geo;\n"
											 "out vec3 Position_geo;\n"
											 "void main()\n"
											 "{\n"
											 "	float u = gl_TessCoord.x, v = gl_TessCoord.y;\n"
											 "	vec3 a = mix(Position_te[0], Position_te[1], u);\n"
											 "	vec3 b = mix(Position_te[2], Position_te[3], u);\n"
											 "	Position_geo = mix(a, b, v);\n" //pass to geometry shader
											 "	vec2 a2 = mix(UV_te[0], UV_te[1], u);\n"
											 "	vec2 b2 = mix(UV_te[2], UV_te[3], u);\n"
											 "	UV_geo = mix(a2, b2, v);\n"
											 "}\n");

			my_shader.uploadGeometry("#version 430\n"
									 //"layout(quads) in;\n"
									 "layout(triangles) in;\n" //can't use quads
									 "layout(triangle_strip, max_vertices=24) out;\n"
									 "in vec3 Position_geo[];\n"
									 "in vec2 UV_geo[];\n"
									 "out vec2 UV_fr;\n"
									 "out vec3 Distance;\n"
									 "layout (binding=2) uniform result { mat4 CameraMatrix[]; }; \n" //gets data from compute shader
									 "void main()\n"
									 "{\n"
									 "	for(int i = 0; i < 3; i++)\n"
									 "	{\n"
									 "		gl_Position = CameraMatrix[0] * vec4(Position_geo[i], 1.0);\n" //again doesn't matter which ModelViewMatrix_geo (should be all the same)
									 "		UV_fr = UV_geo[i];\n"
									 "		if (i == 0) Distance = vec3(1,0,0);\n"
									 "		else if (i == 1) Distance = vec3(0,1,0);\n"
									 "		else Distance = vec3(0,0,1);\n"
									 "		EmitVertex();\n"
									 "	}\n"
									 "	EndPrimitive();\n"
									 //Dup it
									 "	for(int i = 0; i < 3; i++)\n"
									 "	{\n"
									 "		gl_Position = CameraMatrix[0] * vec4(Position_geo[i] + vec3(0,0,0.25), 1.0);\n" //again doesn't matter which ModelViewMatrix_geo (should be all the same)
									 "		UV_fr = UV_geo[i];\n"
									 "		if (i == 0) Distance = vec3(1,0,0);\n"
									 "		else if (i == 1) Distance = vec3(0,1,0);\n"
									 "		else Distance = vec3(0,0,1);\n"
									 "		EmitVertex();\n"
									 "	}\n"
									 "	EndPrimitive();\n"
									 "}\n");

			my_shader.uploadFragment("#version 400\n"
									 "uniform sampler2D texture;\n"
									 "layout(location = 0) out vec4 FragColor;\n"
									 "in vec2 UV_fr;\n"
									 "in vec3 Distance;\n"
									 "void main()\n"
									 "{\n"
									 "	float D = clamp(min(min(Distance.x, Distance.y), Distance.z) / 0.08, 0, 1);\n"
									 "	FragColor = vec4(D,D,D,1) * texture2D(texture, UV_fr);\n"
									 "}\n");

			my_shader.compile();

			my_compute.upload("#version 430\n"
							  "uniform vec3 camera_pos;\n"
							  "uniform vec3 camera_look_at_pos;\n"
							  "layout (binding=2) buffer result { mat4 matrix[]; }; \n" //perverted, layout(binding=4) mat4 result[]; wont work
							  "layout (local_size_x = 1) in;\n"
							  "void main()\n"
							  "{\n"
							  //calculate look up at gpu
							  "	vec3 from = camera_pos;\n" //from to doesn't work right ? no idea why
							  "	vec3 to   = camera_look_at_pos;\n"
							  "	vec3 dir  = normalize(to - from);\n" //could be a "normal" for collisions detection
							  "	vec3 up   = vec3(0,1,0);\n"
							  "	vec3 right= cross(dir, up);\n"
							  "	up = cross(dir, right);\n"
							  "	mat4 look_at_matrix = mat4(vec4(right, 0),\n"
							  "							   vec4(up   , 0),\n"
							  "							   vec4(dir  , 0),\n"
							  "							   vec4(-from, 1));\n"
							  "	matrix[gl_GlobalInvocationID.x] = inverse(look_at_matrix);\n"
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
							  "matrix[gl_GlobalInvocationID.x] = ProjectionMatrix * matrix[gl_GlobalInvocationID.x];"
							  "}\n");

			my_compute.compile();
		}

		bool update()
		{
			my_window.update();

			my_window.begin();

				glClearColor(1,1,1,1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);

				static int frame = 0;
				frame = frame + 1;

				GLfloat pos[3] = {std::cos(frame/100.0f)*10.0f, 10, std::sin(frame/100.0f)*10.0f};
				my_compute.set(&my_camera_pos, 3, 1, pos);
				my_shader.set(&my_texture_pos, &my_texture); //not good this way ?

				my_compute.begin();
					my_compute.bind(&my_buffer); //binds the buffer to storage buffer block binding=0..n
					my_compute.execute(1,1,1);
				my_compute.end();

				my_shader.begin();
					my_shader.bind(&my_buffer); //binds the buffers to uniform block binding=0..n
					my_buffer.render(4, 8);
				my_shader.end();

				my_window.flip(1000/30); //max. 30Hz

			my_window.end();
			return run;
		}
};

int main()
{
	test_window test;

	while(test.update());

	return 0;
}

