#include "Building.h"

int width = 15;
int length = 15;

Building::Building()
{
}

Building::Building(int x, int z, std::shared_ptr<Material> material)
{
	this->damage = 20;
	this->range = 700;

	glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(x, -10.0f, z));
	this->x = position[3][0];
	this->z = position[3][2];
	model = new Geometry(position, Geometry::createCubeGeometry(width, 20, length), material);
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

void Building::draw() 
{
	model->draw();
}