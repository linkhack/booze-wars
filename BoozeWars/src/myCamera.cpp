#include "myCamera.h"



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
	glm::vec3 lookDirection = glm::vec3(glm::cos(yaw), glm::sin(yaw), 0);
	switch (key)
	{
	case GLFW_KEY_W:
		position += movementSpeed * lookDirection;
		break;
	case GLFW_KEY_A:
		break;
	case GLFW_KEY_S:
		position -= movementSpeed * lookDirection;
		break;
	case GLFW_KEY_D:
		break;
	default:
		break;
	}
}
