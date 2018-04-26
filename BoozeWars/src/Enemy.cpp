#include "Enemy.h"

Enemy::Enemy() 
{

}

Enemy::Enemy(std::shared_ptr<Material> material,std::shared_ptr<Street> street)
{
	model = new Geometry(glm::mat4(1.0f), Geometry::createCubeGeometry(10, 10, 10), material);
	glm::mat2 part = street->getPart1();
	x = part[0][0];
	z = 5;
	y = part[0][1]+ 0.5*(street->getStreetWidth());
	this->street = street;
	movementspeed = 25.0f;
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
	return z;
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
	std::cout << hp << std::endl;
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