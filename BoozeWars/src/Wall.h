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



public:
	static const float width;
	Wall(float x, float z, std::shared_ptr<Material> material);
	~Wall();
	PxRigidStatic initPhysics(PxPhysics* physicsSDK);
};

