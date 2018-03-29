#pragma once
#include "Geometry.h"
class Enemy
{
private:
	Geometry *model;
	float x;
	float y;
	float hp;
	float damageTeens; //Amount of drunken teens per bottle

public:
	Enemy();
	Enemy(std::shared_ptr<Material> material);
	~Enemy();

	Enemy(Enemy&&) = default;
	Enemy& operator=(Enemy&&) = default;
	Enemy& operator=(const Enemy &) = default;
	
	float getX();
	float getY();
	float getHP();

	//void transform(float dT);
	void hit(float damage);

	void draw();
	void selfDestruct(); // delete model from world
};

