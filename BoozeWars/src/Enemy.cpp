#include "Enemy.h"

Enemy::Enemy() 
{

}

Enemy::Enemy(std::shared_ptr<Material> material,int x, int y)
{
	model = new Geometry(glm::mat4(1.0f), Geometry::createCubeGeometry(10, 10, 10), material);
	x = 0;
	y = 0;
	z = 15;
	street=Street(x, y);
	movementspeed = 60.0f;
}



Enemy::~Enemy()
{
}

glm::mat4 Enemy::getModelMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(x, -z, y));
}

void Enemy::draw() {
	model->setTransformMatrix(getModelMatrix());
	model->draw();
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
	if (x < street.getPart2()[0][0]) {
		x += movementspeed * dT;
	}

}