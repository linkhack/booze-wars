#include "myCamera.h"
#include <iostream>



myCamera::myCamera(float fov, float aspect, float near, float far)
{

	projMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
	movementSpeed = 50;
	position = glm::vec3(0.0f, 10.0f, 150.0f);
	rotationMatrix = glm::mat4(1.0f);
	yaw = glm::radians(90.0f);
	pitch  =0;
	roll = 0;
	oldX = oldY = 0;
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
	return -glm::vec3(rotationMatrix[0][2], rotationMatrix[1][2], rotationMatrix[2][2]);
}

glm::vec3 myCamera::getOrthodirection()
{
	return glm::vec3(rotationMatrix[0][0], rotationMatrix[1][0], rotationMatrix[2][0]);
}

glm::mat4 myCamera::getRotationMatrix()
{
	return rotationMatrix;
}

glm::mat4 myCamera::getProjectionMatrix()
{
	return projMatrix;
}

glm::mat4 myCamera::getViewProjectionMatrix() {
	//Translation
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), -position);
	
	//Rotations
	glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));


	glm::quat orientation = qPitch * qYaw;
	orientation = glm::normalize(orientation);
	rotationMatrix = glm::mat4_cast(orientation);
	

	//View and proj matrix
	viewMatrix = rotationMatrix*translation;
	return projMatrix*viewMatrix;
}

void myCamera::update(int x, int y, float zoom, bool dragging, bool strafing){}

void myCamera::updatePosition(int key, float dT)
{
	//W:forward
	//A:left
	//S:backwards
	//D:right
	//R:upwards
	//F:downwards
	glm::vec3 orthoDirection = glm::normalize(glm::vec3(rotationMatrix[0][0],0,rotationMatrix[2][0]));
	glm::vec3 lookDirection = -glm::normalize(glm::vec3(rotationMatrix[0][2],0,rotationMatrix[2][2]));
	switch (key)
	{
	case GLFW_KEY_W:
		position += movementSpeed * lookDirection*dT;
		break;
	case GLFW_KEY_A:
		position -= movementSpeed * orthoDirection*dT;
		break;
	case GLFW_KEY_S:
		position -= movementSpeed * lookDirection*dT;
		break;
	case GLFW_KEY_D:
		position += movementSpeed * orthoDirection*dT;
		break;
	case GLFW_KEY_R:
		position.y += movementSpeed * dT;
		break;
	case GLFW_KEY_F:
		if (position.y - movementSpeed * dT >= 1) {
			position.y -= movementSpeed * dT;
		}
		break;
	default:
		break;
	}
	
}

void myCamera::updateDirection(int x, int y)
{	
	int dx = x - oldX;
	int dy = y - oldY;

	yaw += 0.001*dx;
	pitch += 0.001*dy;
	if (pitch >= glm::radians(88.0f)) {
		pitch = glm::radians(88.0f);
	}
	if (pitch <= glm::radians(-88.0f)) {
		pitch = glm::radians(-88.0f);
	}
	//yaw = glm::radians(yaw);
	//pitch = glm::radians(pitch);
	oldX = x;
	oldY = y;
}

/**Return intersection of look direction and y=0 plane, if no such point can be found or it is
* too far away then glm::vec3(0,1,0) is returned;
*
*/
glm::vec3 myCamera::getGroundIntersection()
{
	glm::vec3 lookDirecrtion = -myCamera::getLookDirection();
	glm::vec3 position = myCamera::getPosition();
	float yChange = lookDirecrtion.y;
	float yPosition = position.y;
	float t = -yPosition / yChange;
	if (yChange > 0 && t<100) { 
		return position + t * lookDirecrtion;
	} else {
		return glm::vec3(0, 1, 0);
	}

}
