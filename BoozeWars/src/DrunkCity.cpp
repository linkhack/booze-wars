#include "DrunkCity.h"
#include "Exceptions.h"
#include "Skybox.h"

DrunkCity::DrunkCity()
{

}

DrunkCity::DrunkCity(float x, float y, float z)
{
	limitBuildings = 10;
	citySizeX = x;
	citySizeZ = z;
	
	hp = 100;
	highway = std::make_shared<Street>(x, z);
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
		glm::vec2 buildingCoords = glm::vec2(building->getX(), building->getZ());
		glm::vec2 enemyCoords = glm::vec2(iteratingEnemy->getX(), iteratingEnemy->getZ());
		float iterEnemylength = glm::abs(glm::distance(buildingCoords, enemyCoords));
		if (iterEnemylength >= building->getRange()) //if out of range don't need to check
			continue;
		if (!nearestEnemy) {
			nearestEnemy = iteratingEnemy;
		}
		else {
			glm::vec2 lastEnemyCoords = glm::vec2(nearestEnemy->getX(), nearestEnemy->getZ());
			float lastEnemyLength = glm::abs(glm::distance(buildingCoords, lastEnemyCoords));
			if (lastEnemyLength > iterEnemylength) {
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
	enemiesAlive.push_back(new Enemy(material,highway));
}


void DrunkCity::addBuilding(Building* building)
{
	if (buildings.size() >= limitBuildings) {
		throw BUILDINGS_LIMIT_REACHED;
	}
	buildings.push_back(building);
}

void DrunkCity::fight(float dT)
{
	for (std::list<Building*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		Building* building = *it;
		Enemy* enemy = getNearestEnemy(building);
		enemy->hit(building->getDamage()*dT);
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
			if (iterEnemy->getX() >= 1000 || iterEnemy->getZ()>=1000)
			{
				hp -= iterEnemy->getDamage();
				iterEnemy->selfDestruct();
				it = enemiesAlive.erase(it);
				delete iterEnemy;
				iterEnemy = NULL;

				if (hp <= 0) {
					throw GAME_END;
				}
				
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
	if (isColliding(highway->getPart1(), toPlace) || 
		isColliding(highway->getPart2(), toPlace) || 
		isColliding(highway->getPart3(), toPlace)) {
		throw STREET_COLLISION;
	}

	while (it != buildings.end())
	{
		Building* iterBuilding = *it;
		if (isColliding(
			glm::mat2x2(iterBuilding->getX(), iterBuilding->getZ(), iterBuilding->getX() + iterBuilding->getWidth(), iterBuilding->getZ() + iterBuilding->getLength()),
			toPlace
		)) {
			throw BUILDING_COLLISION;
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

int DrunkCity::getBuildingsLeft()
{
	return limitBuildings - buildings.size();
}
