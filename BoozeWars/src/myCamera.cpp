#include "myCamera.h"
#include <iostream>


myCamera::myCamera(float fov, float aspect, float near, float far)
{
	projMatrix = glm::perspective(fov, aspect, near, far);
	movementSpeed = 0.1;
	position = glm::vec3(-10, 10, 0);
	yaw = 0;
	pitch = 0;
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
	glm::mat4 transform(1);
	transform = glm::translate(transform,transPos);
	transform = glm::rotate(transform, yaw, glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, pitch, glm::vec3(1, 0, 0));
	viewMatrix = transform;
	return projMatrix*transform;
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
	glm::vec3 lookDirection = glm::vec3(glm::sin(yaw), 0, glm::cos(yaw));
	glm::vec3 orthoDirection = glm::vec3(-glm::cos(yaw), 0, glm::sin(yaw));
	switch (key)
	{
	case GLFW_KEY_W:
		position -= movementSpeed * lookDirection;
		break;
	case GLFW_KEY_A:
		position -= movementSpeed * orthoDirection;
		break;
	case GLFW_KEY_S:
		position += movementSpeed * lookDirection;
		break;
	case GLFW_KEY_D:
		position += movementSpeed * orthoDirection;
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
	std::cout << "(" << x << "," << y << ")" << std::endl;
}
