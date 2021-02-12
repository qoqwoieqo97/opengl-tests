#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <fstream>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderFile
{
	std::string vertex, fragment;
};

class Shader
{
private:
	unsigned int id;
public:
	void loadShader(unsigned int shader_type, const char* shader);
	Shader(unsigned int shader_type, const char* shader);
	unsigned int getId();
};

class Program
{
private:
	unsigned int id;
	std::unordered_map<std::string,unsigned int> location_map;
public:
	Shader frag, vert;

	unsigned int static create_program(Shader fragment, Shader vertex);
	ShaderFile static getFile(std::string path);
	Program static createProgram(std::string path);

	void refresh_program(std::string refresh_path);

	Program(Shader fragment, Shader vertex);
	void use() { glUseProgram(id); }

	unsigned int find_Uniform(std::string name);
	void setInt(std::string name, int val);
	void setMat4(std::string name, glm::mat4 value);
	void setVec3(std::string name, float x, float y, float z);
	void setVec3(std::string name, glm::vec3 vector);

	unsigned int getId();
	void setId(unsigned int i);
};