#include "Building.h"

int width = 20;
int length = 20;
int range = 70;

Building::Building()
{
}

Building::Building(float x, float z, std::shared_ptr<Material> material)
{
	this->damage = 45;
	glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(x, -15.0f, z));
	this->x = position[3][0];
	this->z = position[3][2];
	model = new Geometry(position, Geometry::createCubeGeometry(width, 30, length), material);
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
	model->drawShadow(shader);
}

void Building::draw()
{
	model->draw();
}