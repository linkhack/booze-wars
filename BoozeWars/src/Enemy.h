#pragma once
#include "Geometry.h"
class Enemy
{
private:
	Geometry model;
	float x;
	float y;
	float hp;
	float damageTeens; //Amount of drunken teens per bottle

public:
	Enemy(std::shared_ptr<Material> material);
	~Enemy();

	//void transform(float dT);
	//void hit(float damage);

	void draw();
};
