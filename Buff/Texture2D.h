#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <iostream>
#include "../ErrorHandling/error_controlling.h"
#include "../stb_image/stb_image.h"

enum class TextureType { PNG, JPG };

struct Texture2Data
{
	unsigned char* data;
	int width, height, nrChannels;
	Texture2Data(std::string path);
	void image_free();
};

class Texture2D
{
private:
	unsigned int id;
public:
	unsigned int get_format(TextureType type);

	Texture2D(Texture2Data *data, TextureType type);
	void bind(unsigned int text_activate);
	void bind();
};