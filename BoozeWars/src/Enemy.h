#pragma once
#include "Geometry.h"
#include "Street.h"
#include <PhysX/PxPhysicsAPI.h>

using namespace physx;
class Enemy
{
private:
	std::shared_ptr<Geometry> model;
	float x;
	float y;
	float z;
	float hp;
	float movementspeed; //max speed in meter/second
	float damageTeens; //Amount of drunken teens per bottle
	std::shared_ptr<Street> street;
	
	PxRigidDynamic* physxActor;

public:
	Enemy();
	Enemy(std::shared_ptr<Street> Street,std::shared_ptr<Geometry> model);
	~Enemy();

	Enemy(Enemy&&) = default;
	Enemy& operator=(Enemy&&) = default;
	Enemy& operator=(const Enemy &) = default;

	float getX();
	float getZ();
	float getHP();
	float getDamage();
	float getDistanceSquared(Enemy&otherEnemy );
	
	PxVec3 getDesiredDirection();
	void hit(float damage);

	glm::mat4 getModelMatrix();

	void drawShadows(Shader& shader);
	void draw();
	void selfDestruct(); // delete model from world

	PxRigidDynamic* createPhysics(PxPhysics* physicsSDK);
	void applyForce(PxVec3 force);
	void updatePosition();
};

