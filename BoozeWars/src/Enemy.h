#pragma once
#include "Geometry.h"
#include "Street.h"
#include <PhysX/PxPhysicsAPI.h>
#include "Model.h"
#include "ModelFactory.h"

using namespace physx;
class Enemy
{
protected:
	Model* model;
	float x;
	float y;
	float z;
	float hp;
	float movementspeed; //max speed in meter/second
	float damageTeens; //Amount of drunken teens per bottle

	int streetPart;
	std::shared_ptr<Street> street;
	
	PxRigidDynamic* physxActor;

	glm::mat4 scale;

public:
	Enemy();
	Enemy(std::shared_ptr<Street> Street,const int modelNr);
	~Enemy();

	Enemy(Enemy&&) = default;
	Enemy& operator=(Enemy&&) = default;
	Enemy& operator=(const Enemy &) = default;

	float getX();
	float getZ();
	float getHP();
	float getDamage();
	float getDistanceSquared(const Enemy& otherEnemy );

	virtual PxVec3 getDesiredDirection();

	PxVec3 getDirectionVector(const Enemy& otherEnemy);
	void hit(float damage);

	glm::mat4 getModelMatrix();

	void drawShadows(Shader& shader);
	void draw(Shader* shader);
	void selfDestruct(); // delete model from world

	virtual PxRigidDynamic* createPhysics(PxPhysics* physicsSDK);
	void applyForce(PxVec3 force);
	void applyDrivingForce();
	void updatePosition();
};

