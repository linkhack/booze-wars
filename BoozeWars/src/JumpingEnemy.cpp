#include "JumpingEnemy.h"



JumpingEnemy::JumpingEnemy()
{
}

JumpingEnemy::JumpingEnemy(std::shared_ptr<Street> Street, const int modelNr)
	:Enemy(Street,modelNr)
{
	movementspeed = 30.0f;
	hp = 20;
	damageTeens = 10;
	ModelFactory* factory = ModelFactory::Instance();
	model = factory->getModel(ModelFactory::JUMPING_ENEMY);
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f));
}


JumpingEnemy::~JumpingEnemy()
{
}
