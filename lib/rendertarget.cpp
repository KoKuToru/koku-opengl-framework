#include "rendertarget.h"
#include "buffer.h"
#include "texture.h"
#include "window.h"

using namespace koku;
using namespace opengl;

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
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, my_tex[i].id, 0);
			textures.push_back(my_tex[i].id);
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
