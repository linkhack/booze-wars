#include "myCamera.h"
#include <iostream>


myCamera::myCamera(float fov, float aspect, float near, float far)
{
	projMatrix = glm::perspective(fov, aspect, near, far);
	movementSpeed = 0.1;
	position = glm::vec3(0.0f, -10.0f, 0.0f);
	yaw = 0;
	pitch =0;
	roll = 0;
}


myCamera::~myCamera()
{
}


glm::vec3 myCamera::getPosition() 
{
	return position;
}

glm::vec3 myCamera::getLookDirection()
{
	return glm::vec3(glm::cos(yaw), 0, -glm::sin(yaw));
}

glm::vec3 myCamera::getOrthodirection()
{
	return glm::vec3(glm::sin(yaw), 0, glm::cos(yaw));
}

glm::mat4 myCamera::getViewProjectionMatrix() {
	//Translation
	glm::vec3 transPos = position;
	transPos *= -1;
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), transPos);
	
	//Rotations
	glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));

	glm::quat orientation = qPitch * qYaw;
	orientation = glm::normalize(orientation);
	glm::mat4 rotate = glm::mat4_cast(orientation);

	//View and proj matrix
	viewMatrix = rotate*translation;
	return projMatrix*viewMatrix;
}

void myCamera::update(int x, int y, float zoom, bool dragging, bool strafing){}

void myCamera::updatePosition(int key)
{
	//W:forward
	//A:left
	//S:backwards
	//D:right
	//R:upwards
	//F:downwards
	glm::vec3 orthoDirection = getLookDirection();
	glm::vec3 lookDirection = getOrthodirection();
	switch (key)
	{
	case GLFW_KEY_W:
		position += movementSpeed * lookDirection;
		break;
	case GLFW_KEY_A:
		position += movementSpeed * orthoDirection;
		break;
	case GLFW_KEY_S:
		position -= movementSpeed * lookDirection;
		break;
	case GLFW_KEY_D:
		position -= movementSpeed * orthoDirection;
		break;
	case GLFW_KEY_R:
		position.y += movementSpeed;
		break;
	case GLFW_KEY_F:
		position.y -= movementSpeed;
	default:
		break;
	}
}

void myCamera::updateDirection(int x, int y)
{
	yaw = 0.1*x;
	pitch = 0.1*y;
	if (pitch >= 88) {
		pitch = 88;
	}
	if (pitch <= -88) {
		pitch = -88;
	}
	yaw = glm::radians(yaw);
	pitch = glm::radians(pitch);
}


