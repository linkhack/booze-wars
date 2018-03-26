#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>

class myCamera
{
private:
	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;
	float yaw, pitch;
	glm::vec3 position;
	float movementSpeed;

public:
	myCamera(float fov, float aspect, float near, float far);
	~myCamera();

	glm::vec3 getPosition();
	glm::mat4 getViewProjectionMatrix();
	void update(int x, int y, float zoom, bool dragging, bool strafing);
	void updatePosition(int direction);
};

