#pragma once
#include "Enemy.h"
class JumpingEnemy :
	public Enemy
{
public:
	JumpingEnemy();
	JumpingEnemy(std::shared_ptr<Street> Street);
	~JumpingEnemy();

	virtual void applyDrivingForce(PxScene * physxScene);

};

