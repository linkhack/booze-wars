#pragma once
#include "Enemy.h"
class JumpingEnemy :
	public Enemy
{
public:
	JumpingEnemy();
	JumpingEnemy(Street* Street);
	~JumpingEnemy();

	virtual void applyDrivingForce(PxScene * physxScene);

};

