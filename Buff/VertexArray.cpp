#include "VertexArray.h"
VertexArray::VertexArray(VertexBuffer vb,unsigned int size, GLsizei stride, const void* ptr)
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	glEnableVertexAttribArray(0);
	addBuffer(vb, size, stride, ptr);
}

void VertexArray::addBuffer(VertexBuffer vb, unsigned int size, GLsizei stride,const void* ptr)
{
	bind();
	vb.bind();
	glVertexAttribPointer(last_added, size, GL_FLOAT, GL_FALSE, stride, ptr);
	glEnableVertexAttribArray(last_added); last_added++;
}
void VertexArray::unbind()
{
	glBindVertexArray(0);
}

void VertexArray::bind()
{
	glBindVertexArray(id);
}

unsigned int VertexArray::getId()
{
	return id;
}

void VertexArray::draw_array()
{
	bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}