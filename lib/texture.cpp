#include "texture.h"
#include "window.h"

using namespace koku;
using namespace opengl;

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
