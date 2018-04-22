#include "Building.h"

int width = 10;
int length = 10;

Building::Building()
{
}

Building::Building(int x, int z, std::shared_ptr<Material> material)
{
	this->damage = 10;
	this->range = 800;

	glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(x, 1.0f, z));
	this->x = position[3][0];
	this->z = position[3][2];
	model = new Geometry(position, Geometry::createCubeGeometry(width, 10, length), material);
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