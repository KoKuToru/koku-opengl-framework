#include "buffer.h"
#include "window.h"

using namespace koku;
using namespace opengl;

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
