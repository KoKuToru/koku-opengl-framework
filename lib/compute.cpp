#include "compute.h"
#include "buffer.h"

using namespace koku;
using namespace opengl;

compute::compute(window* win): my_shader(win)
{}

void compute::upload(std::string source)
{
	//miss use vertex, is of course our compute shader
	my_shader.upload(GL_COMPUTE_SHADER, my_shader.vertex, source);
}

void compute::compile()
{
	my_shader.compile();
}

void compute::begin()
{
	my_shader.begin();
}

void compute::end()
{
	my_shader.begin();
}

void compute::set(shader_uniform *uniform, int item_count, int count, GLfloat* value)
{
	my_shader.set(uniform, item_count, count, value);
}

void compute::set(shader_uniform* uniform, int item_count, int count, GLint* value)
{
	my_shader.set(uniform, item_count, count, value);
}

void compute::set(shader_uniform *uniform, int item_count, int count, GLuint* value)
{
	my_shader.set(uniform, item_count, count, value);
}

void compute::set(shader_uniform* uniform, GLfloat value)
{
	my_shader.set(uniform, value);
}

void compute::set(shader_uniform* uniform, GLint value)
{
	my_shader.set(uniform, value);
}

void compute::set(shader_uniform* uniform, GLuint value)
{
	my_shader.set(uniform, value);
}

void compute::execute(int num_groups_x, int num_groups_y, int num_groups_z)
{
	glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void compute::bind(buffer *bind_buffe)
{
	for(int i = 0; i < bind_buffe->uploads.size(); ++i)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, bind_buffe->uploads[i]);
	}
}

void compute::set(shader_uniform* uniform, texture* value)
{
	my_shader.set(uniform, value);
}
