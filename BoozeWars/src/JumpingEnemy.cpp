#include "JumpingEnemy.h"



JumpingEnemy::JumpingEnemy()
{
}

JumpingEnemy::JumpingEnemy(std::shared_ptr<Street> Street, const int modelNr)
	:Enemy(Street,modelNr)
{
	movementspeed = 30.0f;
	hp = 20;
	damageTeens = 10;
}


JumpingEnemy::~JumpingEnemy()
{
}

PxVec3 JumpingEnemy::getDesiredDirection()
{
	PxVec3 direction;
	PxVec3 velocity = physxActor->getLinearVelocity();

	if (x < street->getPart1()[1][0] && streetPart == 0) {

		streetPart = 0;
	}
	else if (y < street->getPart2()[1][1] - street->getStreetWidth() && (streetPart == 0 || streetPart == 1)) {
		if (streetPart == 0) {
			physxActor->addForce(PxVec3(-20.0f, 20.0f, 0.0f), PxForceMode::eVELOCITY_CHANGE);
		}
		streetPart = 1;
	}
	else {
		if (streetPart == 1) {
			physxActor->addForce(PxVec3(20.0f, -20.0f, 0.0f), PxForceMode::eVELOCITY_CHANGE);
		}
		streetPart = 2;
	}
	switch (streetPart)
	{
	case 0:
		direction = PxVec3(1.0f, -velocity.y*0.5f + (street->getPart1()[0][1] + 0.5*street->getStreetWidth()) - y, 0.0f);
		break;
	case 1:
		direction = PxVec3(-velocity.x*0.5f + (street->getPart2()[0][0] + 0.5*street->getStreetWidth()) - x, 1.0f, 0.0f);
		break;
	case 2:
		direction = PxVec3(1.0f, -velocity.y*0.5f + (street->getPart3()[0][1] + 0.5*street->getStreetWidth()) - y, 0.0f);
		break;
	default:
		break;
	}
	return direction;
}
