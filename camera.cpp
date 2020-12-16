#include "camera.h"

Camera::Camera(GLfloat radius, GLfloat yaw, GLfloat pitch, GLfloat mouse_sens)
{
	this->radius = radius;

	this->pitch = pitch;
	this->yaw = yaw;

	this->mouse_sens = mouse_sens;

	updateCameraVectors();
}

Camera::~Camera()
{
}


void Camera::rotate(glm::vec2 delta)
{
	this->yaw += delta.x;
	this->pitch += delta.y;
}


void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	/*if (keys[GLFW_KEY_Q] == true)
	{
		radius -= 10.f * deltaTime;

		if (radius < 1.f)
			radius = 1.f;
	}
	
	if (keys[GLFW_KEY_E])
	{
		radius += 10.f * deltaTime;

		if (radius > 20.f)
			radius = 20.f;
	}*/

	updateCameraVectors();
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange, GLfloat scroll)
{
	xChange *= mouse_sens;
	yChange *= mouse_sens;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.f)
		pitch = 89.f;
	if (pitch < -89.f)
		pitch = -89.f;

	if (yaw >= 360.f)
		yaw -= 360.f;
	if (yaw < 0.f)
		yaw += 360.f;

	radius /= std::powf(1.02f, scroll);
	radius = std::max(radius, 10.07f);
	radius = std::min(radius, 30.f);

	updateCameraVectors();
}


glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, target, glm::vec3(0.f, 1.f, 0.f));
}


glm::vec3 Camera::getCameraPosition()
{
	return position;
}
glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(direction);
}


void Camera::updateCameraVectors()
{
	position.x = radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	position.y = radius * sin(glm::radians(pitch));
	position.z = radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	direction = glm::normalize(target - position);

	glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0.f, 1.f, 0.f)));
	up = glm::normalize(glm::cross(right, direction));
}
