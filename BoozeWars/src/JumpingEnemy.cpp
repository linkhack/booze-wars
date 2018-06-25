#include "JumpingEnemy.h"



JumpingEnemy::JumpingEnemy()
{
}

JumpingEnemy::JumpingEnemy(Street* Street)
	:Enemy(Street)
{
	movementspeed = 30.0f;
	hp = 20;
	damageTeens = 10;
	ModelFactory* factory = ModelFactory::Instance();
	model = factory->getModel(ModelFactory::JUMPING_ENEMY);
	modelMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0,-0.5,0))* glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	this->width = 2.3;
	this->length = 2.3;
}


JumpingEnemy::~JumpingEnemy()
{
	physxActor->release();
}

void JumpingEnemy::applyDrivingForce(PxScene* physxScene) {
	PxVec3 velocity = physxActor->getLinearVelocity();
	PxVec3 desiredDirection = getDesiredDirection();
	PxTransform position = physxActor->getGlobalPose();
	PxQueryFilterData filterData(PxQueryFlag::eSTATIC);
	PxSweepBuffer hit1;
	if (physxActor->getLinearVelocity().z<=0.4
		*movementspeed 
		&& physxScene->sweep(PxBoxGeometry(1,1,1),position, desiredDirection, 15, hit1, PxHitFlag::eDEFAULT, filterData))
	{
		physxActor->addForce(PxVec3(0, 0,  movementspeed), PxForceMode::eIMPULSE);
	}

	PxVec3 acceleration = 1.7*(movementspeed * desiredDirection - velocity);
	if (position.p.z <= 3.5f) {
		physxActor->addForce(acceleration*physxActor->getMass());
	}
}