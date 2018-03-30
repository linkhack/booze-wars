#include "Enemy.h"

Enemy::Enemy() 
{

}

Enemy::Enemy(std::shared_ptr<Material> material)
{
	model = new Geometry(glm::mat4(1.0f), Geometry::createCubeGeometry(10, 10, 10), material);
	x = 100;
	y = 100;
	z = 5;
	movementspeed = 15.0f;
}



Enemy::~Enemy()
{
}

glm::mat4 Enemy::getModelMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(x, z, y));
}

void Enemy::draw() {
	model->setTransformMatrix(getModelMatrix());
	model->draw();
}

float Enemy::getX() 
{
	return x;
}

float Enemy::getY() 
{
	return y;
}

float Enemy::getHP()
{
	return hp;
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
	std::cout << dT;
	x += movementspeed * dT;
	std::cout << x << std::endl;
}