#pragma once
#include "Enemy.h"
class JumpingEnemy :
	public Enemy
{
public:
	JumpingEnemy();
	JumpingEnemy(std::shared_ptr<Street> Street, const int modelNr);
	~JumpingEnemy();

	virtual PxVec3 getDesiredDirection();
};

