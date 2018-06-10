#include "Weapon.h"

Weapon::Weapon(glm::mat4 matrix, float delay)
{
	ModelFactory* factory = ModelFactory::Instance();
	model = factory->getModel(ModelFactory::DEFAULT_WEAPON);
	this->modelMatrix = matrix;
	time = 0.0;
	maxTime = 500.0;
	this->delay = delay;
}

Weapon::~Weapon()
{
}

bool Weapon::draw(Shader* shader, float time)
{
	this->time += time;
	if (this->time >= this->delay) {
		model->draw(shader, modelMatrix);
		return this->time >= this->maxTime + this->delay;
	}
	return false;
}

void Weapon::implode()
{
	//TODO: implode particel system
}