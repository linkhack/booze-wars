#include "DrunkCity.h"


DrunkCity::DrunkCity()
{

}

DrunkCity::DrunkCity(float x, float y, float z, std::shared_ptr<Material> material)
{
	limitBuildings = 5;
	citySizeX = x;
	citySizeY = y;
	worldModel = new Geometry(glm::translate(glm::mat4(1.0f),glm::vec3(0,z/2.0f,0))*glm::mat4(1.0f), Geometry::createCubeGeometry(x, z, y), material);
	
	highway = Street(x, y);
}

DrunkCity::~DrunkCity()
{
}

void DrunkCity::zeichne() 
{
	worldModel->draw();

	for (std::list<Enemy*>::iterator it = enemiesAlive.begin(); it != enemiesAlive.end(); ++it)
	{
		Enemy* iteratingEnemy = *it;
		iteratingEnemy->draw();
	}

	for (std::list<Building*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		Building* iteratingBuilding = *it;
		iteratingBuilding->draw();
	}
}

Enemy* DrunkCity::getNearestEnemy(Building* building)
{
	Enemy* nearestEnemy = NULL;
	if (enemiesAlive.size() == 0) {
		//throw Exception
	}
	for (std::list<Enemy*>::iterator it = enemiesAlive.begin(); it != enemiesAlive.end(); ++it)
	{
		Enemy* iteratingEnemy = *it;
		float iteratingEnemyX = abs(iteratingEnemy->getX() - building->getX());
		float iteratingEnemyY = abs(iteratingEnemy->getY() - building->getY());
		if (nearestEnemy && iteratingEnemyX + iteratingEnemyY <= building->getRange()) {
			nearestEnemy = iteratingEnemy;
		}
		else {
			float actualEnemyX = abs(nearestEnemy->getX() - building->getX());
			float actualEnemyY = abs(nearestEnemy->getY() - building->getY());
			if (actualEnemyX + actualEnemyY > iteratingEnemyX + iteratingEnemyY) {
				nearestEnemy = iteratingEnemy;
			}
		}
	}
	if (!nearestEnemy) {
		//throw Exception
	}
	return nearestEnemy;
}

void DrunkCity::addEnemy(std::shared_ptr<Material> material)
{
	enemiesAlive.push_back(new Enemy(material));
}


void DrunkCity::addBuilding(int x, int y, std::shared_ptr<Material> material)
{
	if (buildings.size() >= limitBuildings) {
		// TODO: throw Exception
	}
	buildings.push_back(new Building(x, y, material));
}

void DrunkCity::fight()
{
	for (std::list<Building*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		Building* building = *it;
		Enemy* enemy = getNearestEnemy(building);
		enemy->hit(building->getDamage());
		if (enemy->getHP() <= 0) {
			enemy->selfDestruct();
			enemiesAlive.remove(enemy);
			delete enemy;
			enemy = NULL;
		}
	}
}

void DrunkCity::walk(float dT)
{
	for (std::list<Enemy*>::iterator it = enemiesAlive.begin(); it != enemiesAlive.end(); ++it)
	{
		Enemy* iterEnemy = *it;
		if (iterEnemy != NULL) 
		{
			iterEnemy->walk(dT);
			if (iterEnemy->getX() >= 800 || iterEnemy->getY()>=900)
			{
				iterEnemy->selfDestruct();
				enemiesAlive.remove(iterEnemy);
				delete iterEnemy;
				iterEnemy = NULL;
			}
		}
	}
}