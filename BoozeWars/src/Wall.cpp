#include "Wall.h"

const float Wall::width = 5.0f;


Wall::Wall(float x, float z, std::shared_ptr<Material> material)
{
	this->x = x;
	this->z = z;
	model = new Geometry(glm::mat2(1.0f), Geometry::createCubeGeometry(Wall::width, 3.0f, 0.5f), material);

}

Wall::~Wall()
{
}

PxRigidStatic Wall::initPhysics(PxPhysics * physicsSDK)
{
	return PxRigidStatic();
}
