#include "JumpingEnemy.h"



JumpingEnemy::JumpingEnemy()
{
}

JumpingEnemy::JumpingEnemy(std::shared_ptr<Street> Street)
	:Enemy(Street)
{
	movementspeed = 30.0f;
	hp = 20;
	damageTeens = 10;
	ModelFactory* factory = ModelFactory::Instance();
	model = factory->getModel(ModelFactory::JUMPING_ENEMY);
	modelMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0,-0.5,0))* glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	this->width = 2.0;
	this->length = 2.0;
}


JumpingEnemy::~JumpingEnemy()
{
}
