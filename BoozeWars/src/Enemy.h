#pragma once
#include "Geometry.h"
#include "Street.h"
class Enemy
{
private:
	Geometry *model;
	float x;
	float y;
	float z;
	float hp;
	float movementspeed; //max speed in meter/second
	float damageTeens; //Amount of drunken teens per bottle
	Street street;

public:
	Enemy();
	Enemy(std::shared_ptr<Material> material,int x, int y);
	~Enemy();

	Enemy(Enemy&&) = default;
	Enemy& operator=(Enemy&&) = default;
	Enemy& operator=(const Enemy &) = default;

	float getX();
	float getZ();
	float getHP();
	
	void walk(float dT);
	void hit(float damage);

	glm::mat4 getModelMatrix();

	void draw();
	void selfDestruct(); // delete model from world
};

