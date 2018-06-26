#include "Building.h"

Building::Building()
{
}

Building::Building(float x, float z, int streetDirection)
{
	this->damage = 45;
	glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, z));
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), streetDirection*glm::pi<float>() / 2, glm::vec3(0, 1, 0));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	modelMatrix = position * rotation*scale;
	this->x = position[3][0];
	this->z = position[3][2];
	this->direction = streetDirection;
	ModelFactory* factory = ModelFactory::Instance();
	model = factory->getModel(ModelFactory::DEFAULT_BUILDING);
	this->shootIntervall = 2.0;
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

void Building::drawParticle(Shader* particleShader, Frustum& frustum) {
	std::list<Weapon*>::iterator it = activeWeapons.begin();
	glDisable(GL_CULL_FACE);
	while (it != activeWeapons.end())
	{
		Weapon* weapon = *it;
		if (frustum.sphereInFrustum(weapon->boundingSphere())) {
			weapon->drawParticle(particleShader);
		}
		it++;
	}
	glEnable(GL_CULL_FACE);
}

void Building::draw(Shader* shader, float time, Frustum& frustum)
{	
	if (frustum.sphereInFrustum(boundingSphere())) {
		model->draw(shader, modelMatrix);
	}

	for (std::list<Weapon*>::iterator it = activeWeapons.begin(); it != activeWeapons.end();)
	{

		Weapon* weapon = *it;
		if (weapon->draw(shader, time) || weapon->isHitted()) {
			if (weapon->implode(time) ) {
				it = activeWeapons.erase(it);
				delete weapon;
				weapon = NULL;
			}
			else {
				it++;
			}
		}
		else {
			it++;
		}
	}
}

void Building::shoot(float time) {
	if (this->letItRoll) {
		Weapon* weapon1 = new Weapon(this->x, this->z, this->direction, Weapon::DELAY_FIRST);
		activeWeapons.push_back(weapon1);
		Weapon* weapon2 = new Weapon(this->x, this->z, this->direction, Weapon::DELAY_SECOND);
		activeWeapons.push_back(weapon2);
		Weapon* weapon3 = new Weapon(this->x, this->z, this->direction, Weapon::DELAY_THIRD);
		activeWeapons.push_back(weapon3);
		Weapon* weapon4 = new Weapon(this->x, this->z, this->direction, Weapon::DELAY_FOURTH);
		activeWeapons.push_back(weapon4);
		Weapon* weapon5 = new Weapon(this->x, this->z, this->direction, Weapon::DELAY_FIFTH);
		activeWeapons.push_back(weapon5);
		this->time = time;
		this->letItRoll = false;
	}
	else {
		this->time += time;
		if (this->time > this->shootIntervall) {
			this->time = 0.0;
			this->letItRoll = true;
		}
	}
}


std::list<Weapon*> Building::getWeapons() 
{
	return this->activeWeapons;
}

glm::vec4 Building::boundingSphere()
{
	return glm::vec4(x, 4, z, glm::sqrt(0.25*width * width + 0.25*length * length + 16));;
}

