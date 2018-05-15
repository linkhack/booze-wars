#pragma once
#include "Geometry.h"
#include "Street.h"
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
	
	void walk(float dT);
	void hit(float damage);

	glm::mat4 getModelMatrix();

	void draw();
	void selfDestruct(); // delete model from world
};

