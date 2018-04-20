#include "Building.h"

Building::Building()
{
}

Building::Building(int x, int z, std::shared_ptr<Material> material)
{
	this->x = x;
	this->y = y;
	this->damage = 10;
	this->range = 800;
	this->width = 10;
	this->length = 10;

	glm::mat4 startPoint = glm::mat4(1.0f);
	glm::vec3 moveToCamera = glm::vec3(x, 1.0f, z);
	glm::mat4 resultPosition = glm::translate(startPoint, moveToCamera);
	model = new Geometry(resultPosition, Geometry::createCubeGeometry(width, 10, length), material);
}


Building::~Building()
{
}

int Building::getX()
{
	return x;
}

int Building::getY()
{
	return y;
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