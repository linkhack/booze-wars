#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "myCamera.h"

class Frustum
{
private:
	glm::vec4 planes[6];
	bool cullingActivated;

	float distanceToPoint(glm::vec4 plane, glm::vec3 point);
public:
	Frustum();
	Frustum(myCamera& camera);
	~Frustum();

	void updatePlanes(myCamera& camera);
	bool sphereInFrustum(glm::vec4 centerRadius);
	void useCulling(bool activated);

};

