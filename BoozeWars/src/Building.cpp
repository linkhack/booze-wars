#include "Building.h"

int width = 20;
int length = 20;
int range = 70;

Building::Building()
{
}

Building::Building(float x, float z)
{
	this->damage = 45;
	glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, z));
	glm::mat4 randomRotation = glm::rotate(glm::mat4(1.0f), (rand() % 4)*glm::pi<float>() / 2, glm::vec3(0, 1, 0));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	modelMatrix = position * randomRotation*scale;
	this->x = position[3][0];
	this->z = position[3][2];
	ModelFactory* factory = ModelFactory::Instance();
	model = factory->getModel(ModelFactory::DEFAULT_BUILDING);
	this->shootIntervall = 3000.0;
	this->time = 0.0;
}


Building::~Building()
{
}

int Building::getX()
{
	return x;
}

int Building::getZ()
{
	return z;
}

int Building::getWidth()
{
	return width;
}

int Building::getLength()
{
	return length;
}
/*
Returns damage per second
*/
float Building::getDamage()
{
	return damage;
}

int Building::getRange()
{
	return range;
}

void Building::drawShadows(Shader & shader)
{
	//model->drawShadow(shader);
}

void Building::draw(Shader* shader, float time)
{	
	model->draw(shader, modelMatrix);
	for (std::list<Weapon*>::iterator it = activeWeapons.begin(); it != activeWeapons.end(); ++it)
	{
		Weapon* weapon = *it;
		if (weapon->draw(shader, time)) {
			weapon->implode();
			it = activeWeapons.erase(it);
			delete weapon;
			weapon = NULL;
		}
	}
}

void Building::shoot(float time) {
	if (this->time == 0.0) {
		Weapon* weapon1 = new Weapon(modelMatrix, 0);
		activeWeapons.push_back(weapon1);
		Weapon* weapon2 = new Weapon(modelMatrix, 100);
		activeWeapons.push_back(weapon2);
		Weapon* weapon3 = new Weapon(modelMatrix, 200);
		activeWeapons.push_back(weapon3);
		this->time = time;
	}
	else {
		this->time += time;
		if (this->time > this->shootIntervall) {
			this->time = 0.0;
		}
	}
}