#include "DrunkCity.h"
#include "Exceptions.h"

DrunkCity::DrunkCity()
{

}

DrunkCity::DrunkCity(float x, float y, float z, std::shared_ptr<Material> material)
{
	limitBuildings = 5;
	citySizeX = x;
	citySizeY = y;
	worldModel = new Geometry(glm::mat4(1.0f), Geometry::createCubeGeometry(x, y, z), material);
	
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


void DrunkCity::addBuilding(int x, int y, std::shared_ptr<Material> material)
{
	if (buildings.size() >= limitBuildings) {
		throw BUILDINGS_LIMIT_REACHED;
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
		}
	}
}