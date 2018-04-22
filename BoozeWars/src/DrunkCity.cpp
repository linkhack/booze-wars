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
	citySizeZ = z;
	//Cubemap: right/left/top/bottom/back/front
	hp = 100;
	
	highway = Street(x, z);
}

DrunkCity::~DrunkCity()
{
}

int DrunkCity::getHP() {
	return hp;
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
		float iteratingEnemyZ = abs(iteratingEnemy->getZ() - building->getZ());
		if (iteratingEnemyX + iteratingEnemyZ <= building->getRange())
			continue;
		if (!nearestEnemy) {
			nearestEnemy = iteratingEnemy;
		}
		else {
			float actualEnemyX = abs(nearestEnemy->getX() - building->getX());
			float actualEnemyY = abs(nearestEnemy->getZ() - building->getZ());
			if (iteratingEnemyX + iteratingEnemyZ <= building->getRange() &&
				actualEnemyX + actualEnemyY > iteratingEnemyX + iteratingEnemyZ) {
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
			if (iterEnemy->getX() >= 800 || iterEnemy->getZ()>=900)
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
	glm::mat2x2 toPlace = glm::mat2x2(x, z, x + Building::getWidth(), z + Building::getLength());
	std::list<Building*>::iterator it = buildings.begin();
	if (isColliding(highway.getPart1(), toPlace) || 
		isColliding(highway.getPart2(), toPlace) || 
		isColliding(highway.getPart3(), toPlace)) {
		throw PLACING_COLLISION;
	}

	while (it != buildings.end())
	{
		Building* iterBuilding = *it;
		if (isColliding(
			glm::mat2x2(iterBuilding->getX(), iterBuilding->getZ(), iterBuilding->getX() + iterBuilding->getWidth(), iterBuilding->getZ() + iterBuilding->getLength()),
			toPlace
		)) {
			throw PLACING_COLLISION;
		}
		++it;
	}
	addBuilding(new Building(x, z, material));
}

bool DrunkCity::isColliding(glm::mat2x2 placed, glm::mat2x2 toPlace)
{
	if (placed[0][0] > toPlace[1][0] || 
		placed[0][1] > toPlace[1][1] || 
		placed[1][0] < toPlace[0][0] || 
		placed[1][1] < toPlace[0][1]) {
		return false;
	}
	return true;
}

bool DrunkCity::hasMinOneBuildings()
{
	return buildings.size() > 0;
}