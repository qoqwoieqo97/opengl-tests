#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <iostream>
#include "VertexBuffer.h"
#include "ElementBuffer.h"
class VertexArray
{
private:
	unsigned int id = 0; unsigned int last_added = 0;
public:
	enum class draw_type { WIREFRAME, NORMAL };
	VertexArray(VertexBuffer vb, unsigned int size, GLsizei stride = 0, const void* ptr = 0);
	void addBuffer(VertexBuffer vb, unsigned int size, GLsizei stride = 0, const void* ptr = 0);
	void unbind();
	void bind();
	unsigned int getId();

	void draw_array();
};