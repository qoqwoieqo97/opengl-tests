#include "Shader.h"
#include "../ErrorHandling/error_controlling.h"
#include <glm/gtc/type_ptr.hpp>

void Shader::loadShader(unsigned int shader_type, const char* shader)
{
	id = glCreateShader(shader_type);
	glShaderSource(id, 1, &shader, NULL);
	glCompileShader(id);
	Err::isCompileWorked(id);
}

Shader::Shader(unsigned int shader_type, const char* shader)
{
	loadShader(shader_type, shader);
}
unsigned int Shader::getId() { return id; }

unsigned int Program::create_program(Shader fragment, Shader vertex)
{
	unsigned int id = glCreateProgram();
	glAttachShader(id, fragment.getId());
	glAttachShader(id, vertex.getId());
	glLinkProgram(id);
	return id;
}

ShaderFile Program::getFile(std::string path)
{
	std::string vertex, fragment;
	bool type = false;
	std::fstream file(path); std::string line;
	while (std::getline(file, line))
	{
		if (line == "#fragment") type = false;
		else if (line == "#vertex") type = true;
		else
		{
			if (type == false) fragment += line + "\n";
			else vertex += line + "\n";
		}
	}
	return { vertex,fragment };
}

Program Program::createProgram(std::string path)
{
	ShaderFile file;
	file = Program::getFile(path);
	Shader shader1[2] =
	{ Shader(GL_VERTEX_SHADER, file.vertex.c_str()), Shader(GL_FRAGMENT_SHADER, file.fragment.c_str()) };

	Program program(shader1[0], shader1[1]);
	return program;
}

void Program::refresh_program(std::string refresh_path)
{
	ShaderFile file = Program::getFile(refresh_path);
	Shader s[2] =
	{ Shader(GL_VERTEX_SHADER, file.vertex.c_str()), Shader(GL_FRAGMENT_SHADER, file.fragment.c_str()) };

	unsigned int reloaded_program = Program::create_program(s[0], s[1]);

	if (reloaded_program)
	{
		glDeleteProgram(id); id = reloaded_program;
	}
}

Program::Program(Shader fragment, Shader vertex)
	: frag(fragment), vert(vertex)
{
	id = create_program(fragment, vertex);
	Err::gl_log("Program " + std::to_string(id));
	Err::isLinkingWorked(id);
}

unsigned int Program::find_Uniform(std::string name)
{
	if (location_map.find(name) == location_map.end())
	{
		unsigned int location = glGetUniformLocation(id, name.c_str());
		if (location != -1) location_map[name] = location;
		else
		{
			Err::gl_log("Uniform " + name + " not found.");
			return -1;
		}
	}
	return location_map[name];
}

void Program::setInt(std::string name, int val)
{
	unsigned int loc = find_Uniform(name);
	if (loc != -1) glUniform1i(loc, val);
}

void Program::setMat4(std::string name, glm::mat4 value)
{
	unsigned int loc = find_Uniform(name);
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::setVec3(std::string name, float x, float y, float z)
{
	unsigned int loc = find_Uniform(name);
	if (loc != -1) glUniform3f(loc, x, y, z);
}

void Program::setVec3(std::string name, glm::vec3 vector)
{
	unsigned int loc = find_Uniform(name);
	if (loc != -1) glUniform3f(loc, vector.x, vector.y, vector.z);
}

unsigned int Program::getId() { return id; };
void Program::setId(unsigned int i) { id = i; }