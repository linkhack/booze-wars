#pragma once
#include "Building.h"
#include <PhysX/PxPhysicsAPI.h>

class Wall
{
private:
	Geometry* model;
	PxRigidStatic* physxActor;
	float x;
	float z;
	int streetPart = 0;
	glm::mat4 modelMatrix;

public:
	static const float width;
	static const float height;
	Wall(float x, float z, std::shared_ptr<Material> material, Street* street);
	~Wall();
	PxRigidStatic* initPhysics(PxPhysics* physicsSDK);
	void draw(glm::mat4 modelMatrix=glm::mat4(1.0f));
	float getWidth();
	float getHeight();
	static int getModelMatrix(float x, float z, Street* street);
	glm::vec4 boundingSphere();
};

