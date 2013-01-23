#include "shader.h"
#include "window.h"
#include "buffer.h"
#include "texture.h"

#include <iostream>

using namespace koku;
using namespace opengl;

shader::shader(window* win):
	win(win), programm(-1), vertex(-1), tessellation_control(-1), tessellation_eval(-1), geometry(-1), fragment(-1), version(0)
{}

shader::~shader()
{
	win->begin();
	if (programm != -1)
	{
		glDeleteProgram(programm);
		programm = -1;
	}
	if (vertex != -1)
	{
		glDeleteShader(vertex);
		vertex = -1;
	}
	if (tessellation_control != -1)
	{
		glDeleteShader(tessellation_control);
		tessellation_control = -1;
	}
	if (tessellation_eval != -1)
	{
		glDeleteShader(tessellation_eval);
		tessellation_eval = -1;
	}
	if (geometry != -1)
	{
		glDeleteShader(geometry);
		geometry = -1;
	}
	if (fragment != -1)
	{
		glDeleteShader(fragment);
		fragment = -1;
	}
	win->end();
}

void shader::uploadVertex(std::string source)
{
	upload(GL_VERTEX_SHADER, vertex, source);
}

void shader::uploadTessellationControl(std::string source)
{
	upload(GL_TESS_CONTROL_SHADER, tessellation_control, source);
}

void shader::uploadTessellationEval(std::string source)
{
	upload(GL_TESS_EVALUATION_SHADER, tessellation_eval, source);
}

void shader::uploadGeometry(std::string source)
{
	upload(GL_GEOMETRY_SHADER, geometry, source);
}

void shader::uploadFragment(std::string source)
{
	upload(GL_FRAGMENT_SHADER, fragment, source);
}

void printProgramInfoLog(GLuint obj)
{
	if (obj == -1) return;
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		std::cout << infoLog << std::endl;
		free(infoLog);
	}
}

void shader::compile()
{
	win->begin();
	if (programm != -1)
	{
		glDeleteProgram(programm);
		programm = -1;
	}
	programm = glCreateProgram();
	if (vertex != -1)
	{
		glAttachShader(programm, vertex);
	}
	if (tessellation_control != -1)
	{
		glAttachShader(programm, tessellation_control);
	}
	if (tessellation_eval != -1)
	{
		glAttachShader(programm, tessellation_eval);
	}
	if (geometry != -1)
	{
		glAttachShader(programm, geometry);
	}
	if (fragment != -1)
	{
		glAttachShader(programm, fragment);
	}
	glLinkProgram(programm);

	printProgramInfoLog(programm);

	version += 1; //increase version count -> update shader_uniform's

	win->end();
}

void shader::begin()
{
	win->begin();
	glUseProgram(programm);
}

void shader::end()
{
	glUseProgram(0);
	win->end();
}

void shader::checkUniform(shader_uniform* uniform)
{
	if ((uniform->last_sha != this)||(uniform->last_version != version))
	{
		//take new, shader changed
		uniform->last_sha = this;
		uniform->last_version = version;

		uniform->id = glGetUniformLocation(programm, uniform->name.c_str());
	}
}

void shader::set(shader_uniform *uniform, int item_count, int count, GLfloat* value)
{
	win->begin();
	checkUniform(uniform);
	if (uniform->id != -1)
	{
		switch(item_count)
		{
			case 1:
				glProgramUniform1fv(programm, uniform->id, count, value);
				break;
			case 2:
				glProgramUniform2fv(programm, uniform->id, count, value);
				break;
			case 3:
				glProgramUniform3fv(programm, uniform->id, count, value);
				break;
			case 4:
				glProgramUniform4fv(programm, uniform->id, count, value);
				break;
			default:
				//what ?
				break;
		}
	}
	win->end();
}

void shader::set(shader_uniform *uniform, int item_count, int count, GLint* value)
{
	win->begin();
	checkUniform(uniform);
	if (uniform->id != -1)
	{
		switch(item_count)
		{
			case 1:
				glProgramUniform1iv(programm, uniform->id, count, value);
				break;
			case 2:
				glProgramUniform2iv(programm, uniform->id, count, value);
				break;
			case 3:
				glProgramUniform3iv(programm, uniform->id, count, value);
				break;
			case 4:
				glProgramUniform4iv(programm, uniform->id, count, value);
				break;
			default:
				//what ?
				break;
		}
	}
	win->end();
}

void shader::set(shader_uniform *uniform, int item_count, int count, GLuint *value)
{
	win->begin();
	checkUniform(uniform);
	if (uniform->id != -1)
	{
		switch(item_count)
		{
			case 1:
				glProgramUniform1uiv(programm, uniform->id, count, value);
				break;
			case 2:
				glProgramUniform2uiv(programm, uniform->id, count, value);
				break;
			case 3:
				glProgramUniform3uiv(programm, uniform->id, count, value);
				break;
			case 4:
				glProgramUniform4uiv(programm, uniform->id, count, value);
				break;
			default:
				//what ?
				break;
		}
	}
	win->end();
}

void shader::set(shader_uniform* uniform, GLfloat value)
{
	set(uniform, 1, 1, &value);
}

void shader::set(shader_uniform* uniform, GLint value)
{
	set(uniform, 1, 1, &value);
}

void shader::set(shader_uniform* uniform, GLuint value)
{
	set(uniform, 1, 1, &value);
}

void shader::upload(GLenum what, GLuint &where, std::string source)
{
	win->begin();
	if (where != -1)
	{
		glDeleteShader(where);
		where = -1;
	}
	where = glCreateShader(what);
	const char* source_cstr = source.c_str();
	glShaderSource(where, 1, &source_cstr, 0);
	glCompileShader(where);
	win->end();
}

void shader::bind(buffer *bind_buffe)
{
	for(int i = 0; i < bind_buffe->uploads.size(); ++i)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, i, bind_buffe->uploads[i]);
	}
}

void shader::set(shader_uniform* uniform, texture* value)
{
	//umh
	win->begin();
	glActiveTexture(GL_TEXTURE0/*+id*/);
	glBindTexture(GL_TEXTURE_2D, value->id);
	win->end();
	set(uniform, /*GL_TEXTURE*/0/*+id*/);
}
