#include "Enemy.h"



Enemy::Enemy(std::shared_ptr<Material> material)
{
	model = Geometry(glm::mat4(1.0f), Geometry::createCubeGeometry(10, 10, 10), material);
}


Enemy::~Enemy()
{
}

void Enemy::draw() {
	model.draw();
}