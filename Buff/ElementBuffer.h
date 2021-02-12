#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <iostream>

class ElementBuffer
{
private:
	unsigned int id = 0;
public:
	ElementBuffer(const void* data, unsigned int size);
	void setData(const void* data, unsigned int size);
	void bind();
	void unbind();
	unsigned int getId();
};