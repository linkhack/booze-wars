#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
class myCamera
{
private:
	glm::mat4 projMatrix;
	float yaw, pitch;
	glm::vec3 position;

public:
	myCamera(float fov, float aspect, float near, float far);
	~myCamera();

	glm::vec3 getPosition();
	glm::mat4 getViewProjectionMatrix();
	void update(int x, int y, float zoom, bool dragging, bool strafing);
};

