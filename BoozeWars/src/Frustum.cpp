#include "Frustum.h"


Frustum::Frustum()
{
}

Frustum::Frustum(myCamera& camera)
{
	glm::mat4 viewProj = camera.getViewProjectionMatrix();
	glm::vec4 row1 = glm::row(viewProj, 0);
	glm::vec4 row2 = glm::row(viewProj, 0);
	glm::vec4 row3 = glm::row(viewProj, 0);
	glm::vec4 row4 = glm::row(viewProj, 0);
	planes[0] = row4 + row1;
	planes[1] = row4 - row1;
	planes[2] = row4 + row2;
	planes[3] = row4 - row2;
	planes[4] = row4 + row3;
	planes[5] = row4 - row3;
	cullingActivated = true;
}


Frustum::~Frustum()
{
}

void Frustum::updatePlanes(myCamera& camera)
{
	glm::mat4 viewProj = camera.getViewProjectionMatrix();
	glm::vec4 row1 = glm::row(viewProj, 0);
	glm::vec4 row2 = glm::row(viewProj, 1);
	glm::vec4 row3 = glm::row(viewProj, 2);
	glm::vec4 row4 = glm::row(viewProj, 3);
	planes[0] = row4 + row1;
	planes[1] = row4 - row1;
	planes[2] = row4 + row2;
	planes[3] = row4 - row2;
	planes[4] = row3;
	planes[5] = row4 - row3;
}

bool Frustum::sphereInFrustum(glm::vec4 centerRadius)
{
	float d;
	float radius = centerRadius.w;
	glm::vec3 center = glm::vec3(centerRadius.x, centerRadius.y, centerRadius.z);
	for (int i = 0; i < 6; ++i) {
		d = distanceToPoint(planes[i], center);
		if (d <= -radius) {
			return cullingActivated || false;
		}
	}
	return true;
}

void Frustum::useCulling(bool activated)
{
	cullingActivated = activated;
}


float Frustum::distanceToPoint(glm::vec4 plane, glm::vec3 point)
{
	return glm::dot(plane, glm::vec4(point, 1.0f));
}
