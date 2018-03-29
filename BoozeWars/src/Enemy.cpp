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
}



Enemy::~Enemy()
{
}

glm::mat4 Enemy::getModelMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(x, z, y));
}

void Enemy::draw() {
	model->draw(getModelMatrix());
}