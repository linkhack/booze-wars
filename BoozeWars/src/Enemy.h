#pragma once
#include "Geometry.h"
class Enemy
{
private:
	Geometry *model;
	float x;
	float y;
	float z;
	float hp;
	float damageTeens; //Amount of drunken teens per bottle

public:
	Enemy();
	Enemy(std::shared_ptr<Material> material);
	~Enemy();

	Enemy(Enemy&&) = default;
	Enemy& operator=(Enemy&&) = default;
	Enemy& operator=(const Enemy &) = default;
<<<<<<< HEAD

=======
	
	float getX();
	float getY();
	float getHP();
>>>>>>> a9cdc2c31690b326f07599efd7b3ac92c1c40cf8

	//void transform(float dT);
	void hit(float damage);

	glm::mat4 getModelMatrix();

	void draw();
	void selfDestruct(); // delete model from world
};

