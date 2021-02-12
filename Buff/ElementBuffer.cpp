#include "ElementBuffer.h"
ElementBuffer::ElementBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &id);
	setData(data, size);
}

void ElementBuffer::setData(const void* data, unsigned int size)
{
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void ElementBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}
void ElementBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
unsigned int ElementBuffer::getId() { return id; }