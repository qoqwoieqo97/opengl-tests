#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
	glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 cameraPos;
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 cameraRight = glm::normalize(glm::cross(world_up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraWalkUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	float yaw = -90.0f, pitch = 0;

	float lastX = 400, lastY = 300;

	float cameraSpeed = 5.0f;
	

public:
	Camera(glm::vec3 start_pos = glm::vec3(0.0f,0.0f,0.0f));
	void calc_passed(double passed);
	void move_forward();
	void move_backward();
	void move_left();
	void move_right();
	void add_yaw(float xoffset);
	void add_pitch(float yoffset);

	glm::vec3 get_pos();
	glm::mat4 getView();
};