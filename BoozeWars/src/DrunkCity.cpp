#include "DrunkCity.h"
#include "Exceptions.h"
#include "Skybox.h"

DrunkCity::DrunkCity()
{

}

DrunkCity::DrunkCity(float x, float y, float z)
{
	limitBuildings = 5;
	citySizeX = x;
	citySizeY = y;
	//Cubemap: right/left/top/bottom/back/front

	
	highway = Street(x, y);
}

DrunkCity::~DrunkCity()
{
}

void DrunkCity::zeichne() 
{


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
		throw ALL_ENEMIES_DESTROYED;
	}
	for (std::list<Enemy*>::iterator it = enemiesAlive.begin(); it != enemiesAlive.end(); ++it)
	{
		Enemy* iteratingEnemy = *it;
		float iteratingEnemyX = abs(iteratingEnemy->getX() - building->getX());
		float iteratingEnemyY = abs(iteratingEnemy->getY() - building->getY());
		if (iteratingEnemyX + iteratingEnemyY <= building->getRange())
			continue;
		if (!nearestEnemy) {
			nearestEnemy = iteratingEnemy;
		}
		else {
			float actualEnemyX = abs(nearestEnemy->getX() - building->getX());
			float actualEnemyY = abs(nearestEnemy->getY() - building->getY());
			if (iteratingEnemyX + iteratingEnemyY <= building->getRange() &&
				actualEnemyX + actualEnemyY > iteratingEnemyX + iteratingEnemyY) {
				nearestEnemy = iteratingEnemy;
			}
		}
	}
	if (!nearestEnemy) {
		throw NO_ENEMIES_IN_RANGE;
	}
	return nearestEnemy;
}

void DrunkCity::addEnemy(std::shared_ptr<Material> material)
{
	enemiesAlive.push_back(new Enemy(material));
}


void DrunkCity::addBuilding(Building* building)
{
	if (buildings.size() >= limitBuildings) {
		throw BUILDINGS_LIMIT_REACHED;
	}
	buildings.push_back(building);
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
	std::list<Enemy*>::iterator it = enemiesAlive.begin();
	while(it != enemiesAlive.end())	
	{
		Enemy* iterEnemy = *it;
		if (iterEnemy != NULL) 
		{
			iterEnemy->walk(dT);
			if (iterEnemy->getX() >= 800 || iterEnemy->getY()>=900)
			{
				iterEnemy->selfDestruct();
				it = enemiesAlive.erase(it);
				delete iterEnemy;
				iterEnemy = NULL;
			}else			
			{
				++it;
			}
		}
	}
}

void DrunkCity::placeBuilding(int x, int z, std::shared_ptr<Material> material) {
	std::list<Building*>::iterator it = buildings.begin();
	while (it != buildings.end())
	{
		Building* iterBuilding = *it;
		if (x >= iterBuilding->getX() && x <= iterBuilding->getX() + iterBuilding->getWidth() &&
			z >= iterBuilding->getY() && z <= iterBuilding->getY() + iterBuilding->getLength()) {
			//TODO: check if not on street
			throw PLACING_COLLISION;
		}
		++it;
	}
	addBuilding(new Building(x, z, material));
}