#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\constants.hpp>
#include <GLFW\glfw3.h>
#include <glm\gtc\quaternion.hpp>

class myCamera
{
private:
	glm::mat4 rotationMatrix;
	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;
	float yaw, pitch, roll;
	glm::vec3 position;
	float movementSpeed;
	float rotationSpeed;

public:
	myCamera(float fov, float aspect, float near, float far);
	~myCamera();

	glm::vec3 getPosition();

	glm::vec3 getLookDirection();
	glm::vec3 getOrthodirection();
	glm::mat4 getRotationMatrix();
	glm::mat4 getProjectionMatrix();
	
	glm::mat4 getViewProjectionMatrix();
	void update(int x, int y, float zoom, bool dragging, bool strafing);
	void updatePosition(int direction);
	void updateDirection(int x, int y);
	
};

