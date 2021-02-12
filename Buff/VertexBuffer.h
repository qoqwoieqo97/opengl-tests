#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <iostream>

class VertexBuffer
{
private:
	unsigned int id = 0;
public:
	VertexBuffer(const void* data,unsigned int size);
	void setData(const void* data, unsigned int size);
	void bind();
	void unbind();
	unsigned int getId();
};