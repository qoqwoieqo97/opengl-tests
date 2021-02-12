#include "Texture2D.h"

Texture2Data::Texture2Data(std::string path)
{
	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		Err::gl_log("Failed to load texture " + path);
	}
}

void Texture2Data::image_free()
{
	stbi_image_free(data);
}

unsigned int Texture2D::get_format(TextureType type)
{
	if (type == TextureType::JPG) return GL_RGB;
	else if (type == TextureType::PNG) return GL_RGBA;
	else return 0;
}

Texture2D::Texture2D(Texture2Data *data, TextureType type)
{
	glGenTextures(1, &id);
	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data->data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data->width, data->height, 0, get_format(type), GL_UNSIGNED_BYTE, data->data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		Err::gl_log("Failed to glTexImage2D. Image is empty...");
	}
}

void Texture2D::bind(unsigned int text_activate)
{
	glActiveTexture(text_activate);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}