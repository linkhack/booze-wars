#include "Wall.h"

const float Wall::width = 10.0f;
const float Wall::height = 0.25f;


Wall::Wall(float x, float z, std::shared_ptr<Material> material,Street* street)
{
	this->x = x;
	this->z = z;
	model = new Geometry(glm::mat2(1.0f), Geometry::createCubeGeometry(Wall::width, 8.0f, 0.4f), material);
	streetPart = getModelMatrix(x, z, street);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0, z))*glm::rotate(glm::mat4(1.0f), streetPart*glm::pi<float>() / 2, glm::vec3(0, 1, 0));
}

Wall::~Wall()
{
}

PxRigidStatic* Wall::initPhysics(PxPhysics * physicsSDK)
{
	PxMaterial* wallMat;
	wallMat = physicsSDK->createMaterial(0.5f, 0.5f, 1.0f);
	PxTransform position = PxTransform(PxVec3(x, z, 0),PxQuat(streetPart*glm::pi<float>() / 2,PxVec3(0,0,1)));
	physxActor = PxCreateStatic(*physicsSDK, position, PxBoxGeometry(Wall::width*0.5,0.2f,4.0f), *wallMat);
	return physxActor;
}

void Wall::draw(glm::mat4 modelMatrix)
{
	model->draw(modelMatrix * this->modelMatrix);
}

float Wall::getWidth()
{
	if (streetPart == 1) {
		return Wall::width;
	}
	else {
		return Wall::height;
	}
}

float Wall::getHeight()
{
	if (streetPart == 1) {
		return Wall::height;
	}
	else {
		return Wall::width;
	}
}

int Wall::getModelMatrix(float x, float z, Street* street)
{
	int streetPart = 0;
	if (x < street->getPart1()[1][0] && streetPart == 0)
	{
		streetPart = 1;
	}
	else if (z < street->getPart2()[1][1] - street->getStreetWidth() && (streetPart == 0 || streetPart == 1)) {
		streetPart = 2;
	}
	else {
		streetPart = 1;
	}
	return streetPart;
}