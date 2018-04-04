#include "Building.h"

Building::Building()
{
}

Building::Building(int x, int y, std::shared_ptr<Material> material)
{
	this->x = x;
	this->y = y;
	this->damage = 10;
	this->range = 20;
	model = new Geometry(glm::mat4(1.0f), Geometry::createCubeGeometry(10, 10, 10), material);
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

float Building::getDamage()
{
	return damage;
}

float Building::getRange()
{
	return range;
}

//only translation
glm::mat4 Building::getModelMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(x, -z, y));
}

void Building::draw() 
{
	model->setTransformMatrix(getModelMatrix());
	model->draw();
}