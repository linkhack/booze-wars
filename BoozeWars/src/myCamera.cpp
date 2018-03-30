#include "myCamera.h"
#include <iostream>


myCamera::myCamera(float fov, float aspect, float near, float far)
{
	projMatrix = glm::perspective(fov, aspect, near, far);
	movementSpeed = 0.1;
	position = glm::vec3(-10, -10, 0);
	yaw = glm::pi<float>()/2.0f;
	pitch = -glm::pi<float>() / 2.0f;
	roll = 0;
}


myCamera::~myCamera()
{
}


glm::vec3 myCamera::getPosition() 
{
	return position;
}

glm::mat4 myCamera::getViewProjectionMatrix() {
	glm::vec3 transPos = position;
	transPos *= -1;
	glm::vec3 orthoDirection = glm::vec3(-glm::sin(yaw), 0, glm::cos(yaw));
	glm::mat4 translation;
	glm::mat4 rotationYaw;
	glm::mat4 rotationPitch;

	
	translation = glm::translate(glm::mat4(1.0f), transPos);
	rotationYaw = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0, 1, 0));
	rotationPitch = glm::rotate(glm::mat4(1.0f), pitch, orthoDirection);
	
	
	viewMatrix = glm::transpose(rotationPitch*rotationYaw)*translation;
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
	glm::vec3 orthoDirection = glm::vec3(glm::cos(yaw), 0, glm::sin(yaw));
	glm::vec3 lookDirection = glm::vec3(-glm::sin(yaw), 0, glm::cos(yaw));
	switch (key)
	{
	case GLFW_KEY_W:
		position += movementSpeed * orthoDirection;
		break;
	case GLFW_KEY_A:
		position += movementSpeed * lookDirection;
		break;
	case GLFW_KEY_S:
		position -= movementSpeed * orthoDirection;
		break;
	case GLFW_KEY_D:
		position -= movementSpeed * lookDirection;
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
	yaw = 0.001*x;
	pitch = 0.001*y;
}


