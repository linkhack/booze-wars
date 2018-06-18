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
	float width;
	float length;

	int streetPart;
	Street* street;
	
	PxRigidDynamic* physxActor;

	glm::mat4 modelMatrix;

public:
	static const int Enemy::DEFAULT_ENEMY = 1;
	static const int Enemy::JUMPING_ENEMY = 2;
	Enemy();
	Enemy(Street* Street);
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
	virtual void applyDrivingForce(PxScene * physxScene);
	void updatePosition();
	glm::mat2x2 getModel();
	virtual glm::vec4 boundingSphere();
};

