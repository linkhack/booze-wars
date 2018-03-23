#include "DrunkCity.h"


DrunkCity::DrunkCity()
{

}

DrunkCity::DrunkCity(float x, float y, float z, std::shared_ptr<Material> material)
{
	alive = 0;
	worldModel = Geometry(glm::mat4(1.0f), Geometry::createCubeGeometry(x, y, z), material);
	highway = Street(x, y);
	enemiesAlive = new Enemy[100];
}

DrunkCity::~DrunkCity()
{
}

void DrunkCity::draw() 
{
	worldModel.draw();
	for (int i = 0; i < alive; i++) 
	{
		//enemiesAlive[i].draw();
	}
}

void DrunkCity::addEnemy(std::shared_ptr<Material> material)
{
	enemiesAlive[alive] = Enemy(material);
	alive++;
}