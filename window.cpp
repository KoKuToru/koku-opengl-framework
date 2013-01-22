#include "window.h"
#include <iostream>
#include <signal.h>

using namespace koku;
using namespace opengl;

int window::sdl_count = 0;

void debugOutput(unsigned int source, unsigned int type, unsigned int id,
				 unsigned int severity, int length,
				 const char* message, void* userParam)
{
	if (std::string(message, std::string("Buffer detailed info").size()) == "Buffer detailed info")
	{
		//filter it
		return;
	}

	std::string error_source, error_type, error_severity;
	switch(source)
	{
		case GL_DEBUG_SOURCE_API_ARB:
			error_source = "OpenGL";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
			error_source = "Windows";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
			error_source = "Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
			error_source = "Third Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION_ARB:
			error_source = "Application";
			break;
		case GL_DEBUG_SOURCE_OTHER_ARB:
			error_source = "Other";
			break;
		default:
			error_source = "UNKNOW";
			break;
	}

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR_ARB:
			error_type = "Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
			error_type = "Deprecated behavior";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
			error_type = "Undefined behavior";
			break;
		case GL_DEBUG_TYPE_PORTABILITY_ARB:
			error_type = "Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE_ARB:
			error_type = "Performance";
			break;
		case GL_DEBUG_TYPE_OTHER_ARB:
			error_type = "Other";
		default:
			error_type = "UNKNOW";
			break;
	}

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH_ARB:
			error_severity = "High";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM_ARB:
			error_severity = "Medium";
			break;
		case GL_DEBUG_SEVERITY_LOW_ARB:
			error_severity = "Low";
			break;
		default:
			error_severity = "UNKNOW";
			break;
	}

	std::cout << "Source: " << error_source << "  Type: " << error_type << "  Severity: " << error_severity << "  Message: " << message << std::endl;

	if (severity == GL_DEBUG_SEVERITY_HIGH_ARB)
	{
		raise(SIGTRAP);
	}
}

window::window(windowCallback *callback, std::string title, int width, int height, bool vsync, bool resizeable):
	callback(callback), win(nullptr), win_w(width), win_h(height), ren(nullptr), ctx(nullptr)
{
	if (sdl_count == 0)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		{
			throw std::string(SDL_GetError());
		}
	}
	sdl_count += 1;

	win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_w, win_h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (resizeable?SDL_WINDOW_RESIZABLE:0));
	if (win == nullptr)
	{
		throw std::string(SDL_GetError());
	}

	/*
	 *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | (vsync?SDL_RENDERER_PRESENTVSYNC:0));
	if (ren == nullptr)
	{
		throw std::string(SDL_GetError());
	}
	*/

	//OpenGL 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	ctx = SDL_GL_CreateContext(win);
	if (ctx == nullptr)
	{
		throw std::string(SDL_GetError());
	}

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageCallbackARB(debugOutput, NULL);
}

window::~window()
{
	if (ctx != nullptr)
	{
		SDL_GL_DeleteContext(ctx);
		ctx = nullptr;
	}

	if (ren != nullptr)
	{
		SDL_DestroyRenderer(ren);
		ren = nullptr;
	}

	if (win != nullptr)
	{
		SDL_DestroyWindow(win);
		win = nullptr;
	}

	if (--sdl_count == 0)
	{
		//last sdl window
		SDL_Quit();
	}
}

void window::update()
{
	//handle events
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_QUIT:
				//or not ? see SQL_WINDOWEVENT. SDL_WINDOWEVENT_CLOSE
				callback->onQuit();
				break;
			case SDL_WINDOWEVENT:
				if (e.window.windowID == SDL_GetWindowID(win))
				{
					switch (e.window.event)
					{
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							win_w = e.window.data1;
							win_h = e.window.data2;
							callback->onResize(win_w, win_h);
							break;
						case SDL_WINDOWEVENT_CLOSE:
							callback->onQuit();
							break;
						default:
							break;
					}
				}
				else
				{
					//TODO .. handle multiply windows ??
					//what should I do ? now ?
				}
				break;
			default:
				break;
		}
	}
}

void window::flip(int ms_wait)
{
	//SDL_RenderPresent(ren);
	SDL_GL_SwapWindow(win);
	if (ms_wait > 0)
	{
		SDL_Delay(ms_wait);
	}
}

void window::begin()
{
	SDL_GL_MakeCurrent(win, ctx);
}

void window::end()
{
	//nothing atm
}

buffer::buffer(window *win):
	win(win), id(0)
{
	win->begin();
	glGenVertexArrays(1, &id);
	//TODO error check
	win->end();
}

buffer::~buffer()
{
	win->begin();
	for(int i = 0; i < uploads.size(); ++i)
	{
		GLuint tmp = uploads[i];
		glDeleteBuffers(1, &tmp);
	}
	uploads.clear();
	glDeleteVertexArrays(1, &id);
	win->end();
}

void buffer::upload(bool element, const float *data, int size, int item_size)
{
	win->begin();
	GLuint tmp;
	glBindVertexArray(id);
	glGenBuffers(1, &tmp);
	glBindBuffer(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, tmp);
	glBufferData(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, sizeof(float)*size, data, GL_STATIC_DRAW);
	if (element == false)
	{
		glEnableVertexAttribArray(uploads.size());
		glVertexAttribPointer(uploads.size(), item_size, GL_FLOAT, GL_FALSE, 0, 0);
	}
	glBindVertexArray(0);
	glBindBuffer(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, 0);
	uploads.push_back(tmp);
	win->end();
}

void buffer::upload(bool element, const unsigned short *data, int size, int item_size)
{
	win->begin();
	GLuint tmp;
	glBindVertexArray(id);
	glGenBuffers(1, &tmp);
	glBindBuffer(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, tmp);
	glBufferData(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, sizeof(unsigned short)*size, data, GL_STATIC_DRAW);
	if (element == false)
	{
		glEnableVertexAttribArray(uploads.size());
		glVertexAttribPointer(uploads.size(), item_size, GL_SHORT, GL_FALSE, 0, 0);
	}
	glBindVertexArray(0);
	glBindBuffer(element?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER, 0);
	uploads.push_back(tmp);
	win->end();
}

void buffer::render(int vertex_per_face, int count)
{
	win->begin();
	glBindVertexArray(id);
	//index should be short... lets hope for right usage..
	switch (vertex_per_face)
	{
		case 1:
			glDrawElements(GL_POINTS, count, GL_UNSIGNED_SHORT, 0);
			break;
		case 2:
			glDrawElements(GL_LINES, count, GL_UNSIGNED_SHORT, 0);
			break;
		case 3:
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);
			break;
		default:
			glPatchParameteri(GL_PATCH_VERTICES, vertex_per_face);
			glDrawElements(GL_PATCHES, count, GL_UNSIGNED_SHORT, 0);
	}
	glBindVertexArray(0);
	win->end();
}

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

void shader::bind(buffer *bind_buffe)
{
	for(int i = 0; i < bind_buffe->uploads.size(); ++i)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, i, bind_buffe->uploads[i]);
	}
}

texture::texture(window *win): win(win), id(0), w(0), h(0)
{
	win->begin();
	glGenTextures(1, &id);
	win->end();
}

texture::~texture()
{
	win->begin();
	glDeleteTextures(1, &id);
	win->end();
}

void texture::upload(const char *image, int width, int height, int bytes_per_pixel)
{
	w = width;
	h = height;
	win->begin();
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

	switch(bytes_per_pixel)
	{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_BYTE, image);
			break;
		case 2:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, w, h, 0, GL_RG, GL_BYTE, image);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_BYTE, image);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_BYTE, image);
			break;
		default:
			//error !
			break;
	}
	if (image != 0)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	win->end();
}

void texture::upload(const unsigned char *image, int width, int height, int bytes_per_pixel)
{
	w = width;
	h = height;
	win->begin();
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

	switch(bytes_per_pixel)
	{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, image);
			break;
		case 2:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, w, h, 0, GL_RG, GL_UNSIGNED_BYTE, image);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			break;
		default:
			//error !
			break;
	}
	if (image != 0)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	win->end();
}

void texture::upload(const float *image, int width, int height, int bytes_per_pixel)
{
	w = width;
	h = height;
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

	switch(bytes_per_pixel)
	{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, w, h, 0, GL_RED, GL_FLOAT, image);
			break;
		case 2:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, w, h, 0, GL_RG, GL_FLOAT, image);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0, GL_RGB, GL_FLOAT, image);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, image);
			break;
		default:
			//error !
			break;
	}
	if (image != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	win->end();
}

void compute::set(shader_uniform* uniform, texture* value)
{
	my_shader.set(uniform, value);
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

rendertarget::rendertarget(window* win): win(win), rbo_id(0), fbo_id(0), w(-1), h(-1)
{}

rendertarget::~rendertarget()
{
	win->begin();
	if ((w != -1)||(h != -1))
	{
		w = -1;
		h = -1;

		glDeleteRenderbuffers(1, &rbo_id);
		glDeleteFramebuffers(1, &fbo_id);
	}
	win->end();
}

void rendertarget::bind(texture *my_tex, int tex_count)
{
	win->begin();
	if ((w != my_tex->w) || (h != my_tex->h))
	{
		if ((w != -1)||(h != -1))
		{
			w = -1;
			h = -1;

			glDeleteRenderbuffers(1, &rbo_id);
			glDeleteFramebuffers(1, &fbo_id);
		}
		glGenRenderbuffers(1, &rbo_id);
		glGenFramebuffers(1, &fbo_id);
		w = my_tex->w;
		h = my_tex->h;
		glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
		for(int i = 0; i < tex_count; ++i)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, my_tex[0].id, 0);
			textures.push_back(my_tex[0].id);
		}
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_id);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	win->end();
}

void rendertarget::begin()
{
	win->begin();
	glGetIntegerv(GL_VIEWPORT, v_dim);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glViewport(0, 0, w, h);
	win->end();
}

void rendertarget::end()
{
	win->begin();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(v_dim[0], v_dim[1], v_dim[2], v_dim[3]);
	for(int i = 0; i < textures.size(); ++i)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	win->end();
	textures.clear();
}
