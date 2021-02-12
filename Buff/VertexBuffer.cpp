#include "VertexBuffer.h"
VertexBuffer::VertexBuffer(const void* data,unsigned int size)
{
	glGenBuffers(1, &id);
	setData(data, size);
}

void VertexBuffer::setData(const void* data, unsigned int size)
{
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}
void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
unsigned int VertexBuffer::getId() { return id; }