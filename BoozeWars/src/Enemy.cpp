#include "Enemy.h"

Enemy::Enemy() 
{

}

Enemy::Enemy(std::shared_ptr<Material> material,std::shared_ptr<Street> street)
{
	model = new Geometry(glm::mat4(1.0f), Geometry::createCubeGeometry(4, 6, 4), material);
	glm::mat2 part = street->getPart1();
	x = part[0][0];
	z = 3;
	y = part[0][1]+ 0.5*(street->getStreetWidth());
	this->street = street;
	movementspeed = 20.0f;
	hp = 100;
	damageTeens = 5;
}



Enemy::~Enemy()
{
}

glm::mat4 Enemy::getModelMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(x, -z, y));
}

void Enemy::draw() {

	model->draw(getModelMatrix());
}

float Enemy::getX() 
{
	return x;
}

float Enemy::getZ() 
{
	return y;
}

float Enemy::getHP()
{
	return hp;
}
/*
	Returns damage given
*/
float Enemy::getDamage() {
	return damageTeens;
}
void Enemy::hit(float damage)
{	
	hp -= damage;
}

void Enemy::selfDestruct()
{
	if (model != NULL)
	{
		delete model;
		model = NULL;
	}
}

void Enemy::walk(float dT)
{
	if (x < street->getPart1()[1][0] + 0.5*street->getStreetWidth()) {
		x += movementspeed * dT;
	}
	else if (y < street->getPart2()[1][1] - 0.6*street->getStreetWidth()) {
		y += movementspeed * dT;
	}
	else {
		x += movementspeed * dT;
	}
}