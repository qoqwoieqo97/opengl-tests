#include "Camera.h"

Camera::Camera(glm::vec3 startPos)
{
	cameraPos = startPos;
}

void Camera::calc_passed(double passed)
{
	cameraSpeed = 5.0f * passed;
}

void Camera::move_forward()
{
	cameraPos += cameraSpeed * glm::normalize(cameraFront);
}

void Camera::move_backward()
{
	cameraPos -= cameraSpeed * glm::normalize(cameraFront);
}

void Camera::move_left()
{
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraWalkUp)) * cameraSpeed;
}

void Camera::move_right()
{
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraWalkUp)) * cameraSpeed;
}

void Camera::add_yaw(float xoffset)
{
	yaw += xoffset;
}

void Camera::add_pitch(float yoffset)
{
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}


glm::vec3 Camera::get_pos()
{
	return cameraPos;
}

glm::mat4 Camera::getView()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraWalkUp);
}
