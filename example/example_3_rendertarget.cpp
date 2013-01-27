#include "framework.h"
namespace sample_texture
{
	#include "texture.h"
}
#include <cmath>

class test_window: public koku::opengl::windowCallback
{
	private:
		koku::opengl::window my_window;
		koku::opengl::buffer my_buffer;
		koku::opengl::shader my_shader;
		koku::opengl::compute my_compute;
		koku::opengl::shader_uniform my_camera_pos;
		koku::opengl::texture my_texture;
		koku::opengl::shader_uniform my_texture_pos;
		koku::opengl::rendertarget my_rendertraget;
		koku::opengl::texture my_rendertarget_texture;
		bool run;

	protected:
		void onQuit()
		{
			//stop everything !
			run = false;
		}

	public:
		test_window():
			my_window(this, "Example 2 - Texture", 1024, 768, true),
			my_buffer(&my_window),
			my_shader(&my_window),
			my_compute(&my_window),
			my_camera_pos("camera_pos"),
			my_texture(&my_window),
			my_texture_pos("texture"),
			my_rendertraget(&my_window),
			my_rendertarget_texture(&my_window),
			run(true)
		{
			const float vertex[] =
			{
				//FRONT:
				-0.5, -0.5, -0.5, //0
				-0.5,  0.5, -0.5, //1
				 0.5, -0.5, -0.5, //3
				 0.5,  0.5, -0.5, //2
				//BACK:
				 0.5, -0.5,  0.5, //4
				 0.5,  0.5,  0.5, //5
				-0.5, -0.5,  0.5, //6
				-0.5,  0.5,  0.5, //7
				//RIGHT:
				 0.5, -0.5, -0.5, //8
				 0.5,  0.5, -0.5, //9
				 0.5, -0.5,  0.5, //10
				 0.5,  0.5,  0.5, //11
				//LEFT:
				-0.5, -0.5,  0.5, //12
				-0.5,  0.5,  0.5, //13
				-0.5, -0.5, -0.5, //14
				-0.5,  0.5, -0.5, //15
				//TOP:
				 0.5,  0.5,  0.5, //16
				 0.5,  0.5, -0.5, //17
				-0.5,  0.5,  0.5, //18
				-0.5,  0.5, -0.5, //19
				//BOTTOM:
				 0.5, -0.5, -0.5, //20
				 0.5, -0.5,  0.5, //21
				-0.5, -0.5, -0.5, //22
				-0.5, -0.5,  0.5, //23
			};

			const float camera_matrix[] =
			{
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};

			//uv coordinates for texture:
			const float uv[] =
			{
				//FRONT:
				0, 0, 0, 1, 1, 0, 1, 1,
				//BACK:
				0, 0, 0, 1, 1, 0, 1, 1,
				//RIGHT:
				0, 0, 0, 1, 1, 0, 1, 1,
				//LEFT:
				0, 0, 0, 1, 1, 0, 1, 1,
				//TOP:
				0, 0, 0, 1, 1, 0, 1, 1,
				//BOTTOM:
				0, 0, 0, 1, 1, 0, 1, 1
			};

			const unsigned short index[] =
			{
				//FRONT:
				 0, 1, 2, 3,
				//BACK:
				 4, 5, 6, 7,
				//RIGHT:
				 8, 9,10,11,
				//LEFT:
				12,13,14,15,
				//TOP:
				16,17,18,19,
				//BOTTOM:
				20,21,22,23
			};

			//Upload data to buffer
			my_buffer.upload(false,        vertex, 24*3, 3); //layout(location = 0)
			my_buffer.upload(false, camera_matrix,  4*4, 1); //layout(binding  = 1)
			my_buffer.upload(false,            uv, 24*2, 2); //layout(location = 2)
			my_buffer.upload(true,          index, 24*1, 1);

			//Simple vertex-shader:
			my_shader.uploadVertex("#version 400\n"
								   "layout(location = 0) in vec3 Position;\n"
								   "layout(location = 2) in vec2 UV;\n"
								   "out vec3 Position_tc;\n"
								   "out vec2 UV_tc;\n"
								   "void main()\n"
								   "{\n"
								   "	Position_tc = Position;\n"
								   "	UV_tc = UV;\n"
								   "}\n");

			//Simple tessellation-control-shader:
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
												"		gl_TessLevelInner[0] = 2;\n" //tes-level 1 == nothing
												"		gl_TessLevelInner[1] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[0] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[1] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[2] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[3] = gl_TessLevelInner[0];\n"
												"	}\n"
												"}\n");

			//Simple tessellation-eval-shader:
			my_shader.uploadTessellationEval("#version 400\n"
											 "layout(quads, equal_spacing) in;\n"
											 "in vec3 Position_te[];\n"
											 "in vec2 UV_te[];\n"
											 "out vec3 Position_geo;\n"
											 "out vec2 UV_geo;\n"
											 "void main()\n"
											 "{\n"
											 "	float u = gl_TessCoord.x, v = gl_TessCoord.y;\n"
											 "	vec3 a = mix(Position_te[0], Position_te[1], u);\n"
											 "	vec3 b = mix(Position_te[2], Position_te[3], u);\n"
											 "	Position_geo = mix(a, b, v);\n"
											 "	vec2 au = mix(UV_te[0], UV_te[1], u);\n"
											 "	vec2 bu = mix(UV_te[2], UV_te[3], u);\n"
											 "	UV_geo = mix(au, bu, v);\n"
											 "}\n");

			//Simple geometry-shader:
			my_shader.uploadGeometry("#version 430\n"
									 "layout(triangles) in;\n"
									 "layout(triangle_strip, max_vertices=3) out;\n"
									 "in vec3 Position_geo[];\n"
									 "in vec2 UV_geo[];\n"
									 "out vec2 UV_fragment;\n"
									 "out vec3 Distance_fragment;\n"
									 "layout (binding=1) uniform result { mat4 CameraMatrix[]; }; \n"
									 "void main()\n"
									 "{\n"
									 "	for(int i = 0; i < 3; i++)\n"
									 "	{\n"
									 "		gl_Position = CameraMatrix[0] * vec4(Position_geo[i], 1);\n"
									 "		UV_fragment = UV_geo[i];\n"
									 //For wireframe-mode:
									 "		if (i == 0) Distance_fragment = vec3(1,0,0);\n"
									 "		else if (i == 1) Distance_fragment = vec3(0,1,0);\n"
									 "		else Distance_fragment = vec3(0,0,1);\n"
									 "		EmitVertex();\n"
									 "	}\n"
									 "	EndPrimitive();\n"
									 "}\n");

			my_shader.uploadFragment("#version 400\n"
									 "layout(location = 0) out vec4 FragColor;\n"
									 "in vec2 UV_fragment;\n"
									 "in vec3 Distance_fragment;\n"
									 "uniform sampler2D texture;\n"
									 "void main()\n"
									 "{\n"
									 "	float D = clamp(min(min(Distance_fragment.x, Distance_fragment.y), Distance_fragment.z), 0.0, 1.0);\n"
									 "	if (D < 0.05) D = 0.6; else D = 1;\n"
									 "	FragColor = texture2D(texture, UV_fragment) * vec4(D,D,D,1);"
									 "}\n");

			my_shader.compile();

			//Simple compute-shader: (Calculated the camera-matrix)
			my_compute.upload("#version 430\n"
							  "uniform vec3 camera_pos;\n"
							  "uniform vec3 camera_look_at_pos;\n"
							  "layout (binding=1) buffer result { mat4 matrix[]; }; \n"
							  "layout (local_size_x = 1) in;\n"
							  "void main()\n"
							  "{\n"
							  //calculate look up at gpu
							  "	vec3 from = camera_pos;\n"
							  "	vec3 to   = camera_look_at_pos;\n"
							  "	vec3 dir  = normalize(to - from);\n"
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

			//Upload texture:

			//1. decode image (texture.h generated by gimp)
			char* image = new char[sample_texture::width*sample_texture::height*3];
			char* data  = sample_texture::header_data;
			for(int i = 0; i < sample_texture::width*sample_texture::height; ++i)
			{
				HEADER_PIXEL(data, (image+i*3));
			}

			//2. upload image
			my_texture.upload((unsigned char*)image, sample_texture::width, sample_texture::height, 3);

			//Empty texture for rendertarget
			my_rendertarget_texture.upload((unsigned char*)0, 256, 256, 4);
		}

		bool update()
		{
			my_window.update();

			my_window.begin();

				glClearColor(1,1,1,1);
				glEnable(GL_DEPTH_TEST);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				static int frame = -1000;
				frame = frame + 1;

				//RENDER TO TEXTURE:
				{
					my_rendertraget.bind(&my_rendertarget_texture);
					my_rendertraget.begin();
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						GLfloat pos[3] = {std::cos(frame/25.0f)*4.0f, 2, std::sin(frame/25.0f)*4.0f};
						my_compute.set(&my_camera_pos, 3, 1, pos);

						//1. caluclate camera-matrix
						my_compute.begin();
							my_compute.bind(&my_buffer); //binds the buffer to storage buffer block binding=0..n
							my_compute.execute(1,1,1);
						my_compute.end();

						//2. render scene
						my_shader.begin();
							my_shader.set(&my_texture_pos, &my_texture);
							my_shader.bind(&my_buffer); //binds the buffers to uniform block binding=0..n
							my_buffer.render(4, 24);
						my_shader.end();
					my_rendertraget.end();
				}

				//RENDER TO SCREEN:
				{
					GLfloat pos[3] = {std::cos(frame/250.0f)*4.0f, 2, 4+std::cos(frame/100.0f)*2.0f + std::sin(frame/250.0f)*4.0f};
					my_compute.set(&my_camera_pos, 3, 1, pos);

					//1. caluclate camera-matrix
					my_compute.begin();
						my_compute.bind(&my_buffer); //binds the buffer to storage buffer block binding=0..n
						my_compute.execute(1,1,1);
					my_compute.end();

					//2. render scene
					my_shader.begin();
						my_shader.set(&my_texture_pos, &my_rendertarget_texture);
						my_shader.bind(&my_buffer); //binds the buffers to uniform block binding=0..n
						my_buffer.render(4, 24);
					my_shader.end();
				}
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

