#include "myCamera.h"



myCamera::myCamera(float fov, float aspect, float near, float far)
{
	projMatrix = glm::perspective(fov, aspect, near, far);
}


myCamera::~myCamera()
{
}

