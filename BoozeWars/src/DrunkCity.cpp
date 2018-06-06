#include "DrunkCity.h"
#include "Exceptions.h"
#include "Skybox.h"

DrunkCity::DrunkCity()
{

}

DrunkCity::DrunkCity(float x, float y, float z)
{
	limitBuildings = 3;
	citySizeX = x;
	citySizeZ = z;
	hp = 100;
	highway = std::make_shared<Street>(x, z);
}

DrunkCity::~DrunkCity()
{
	gScene->release();
}

int DrunkCity::getHP() {
	return hp;
}
void DrunkCity::drawShadows(Shader& shader) {
	for (std::list<Enemy*>::iterator it = enemiesAlive.begin(); it != enemiesAlive.end(); ++it)
	{
		(*it)->drawShadows(shader);
	}

	for (std::list<Building*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		(*it)->drawShadows(shader);
	}
}
void DrunkCity::zeichne(std::shared_ptr<Shader> shader)
{

	for (std::list<Enemy*>::iterator it = enemiesAlive.begin(); it != enemiesAlive.end(); ++it)
	{
		Enemy* iteratingEnemy = *it;
		iteratingEnemy->draw();
	}

	for (std::list<Building*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		shader->use();
		Building* iteratingBuilding = *it;
		iteratingBuilding->draw(shader);
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

	return nearestEnemy;
}

void DrunkCity::addEnemy(std::shared_ptr<Geometry> model)
{
	Enemy* enemyToAdd = new Enemy(highway, model);
	PxRigidDynamic* dyn = enemyToAdd->createPhysics(gPhysicsSDK);
	enemiesAlive.push_back(enemyToAdd);
	gScene->addActor(*dyn);
}

PxScene* DrunkCity::initPhysics(PxPhysics* gPhysicsSDK)
{
	this->gPhysicsSDK = gPhysicsSDK;
	PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, 0.0f, -9.81f);
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysicsSDK->createScene(sceneDesc);
	return gScene;
}


void DrunkCity::addBuilding(Building* building)
{
	if (buildings.size() >= (int)limitBuildings) {
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
		if(enemy!=nullptr)
		{
			enemy->hit(building->getDamage()*dT);
			if (enemy->getHP() <= 0) {
				limitBuildings += 0.1;
				enemy->selfDestruct();
				enemiesAlive.remove(enemy);
				delete enemy;
				enemy = NULL;
			}
		}
	}
}

void DrunkCity::walk()
{
	std::list<Enemy*>::iterator it = enemiesAlive.begin();
	while(it != enemiesAlive.end())	
	{
		Enemy* iterEnemy = *it;
		if (iterEnemy != NULL) 
		{
			iterEnemy->updatePosition();
			if (iterEnemy->getX() >= 300 || iterEnemy->getZ()>=300)
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

void DrunkCity::calculateForces()
{
	std::list<Enemy*>::iterator it = enemiesAlive.begin();
	while(it != enemiesAlive.end())	
	{
		Enemy* iterEnemy = *it;
		iterEnemy->applyForce(iterEnemy->getDesiredDirection());
		std::list<Enemy*>::iterator otherEnemyIt = enemiesAlive.begin();
		while (otherEnemyIt != enemiesAlive.end())
		{
			if (otherEnemyIt != it) {
				Enemy* otherEnemy = *otherEnemyIt;
				iterEnemy->applyForce(-1.0f*iterEnemy->getDirectionVector(*otherEnemy) / iterEnemy->getDistanceSquared(*otherEnemy));
			}
			++otherEnemyIt;
		}
		++it;
	}
}

void DrunkCity::placeBuilding(float x, float z) {
	glm::mat2x2 toPlace = glm::mat2x2(x - Building::getWidth() / 2, z - Building::getLength() / 2, x + Building::getWidth()/2, z + Building::getLength()/2);
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
			glm::mat2x2(iterBuilding->getX() - iterBuilding->getWidth() / 2, iterBuilding->getZ() - iterBuilding->getLength() / 2, iterBuilding->getX() + iterBuilding->getWidth()/2, iterBuilding->getZ() + iterBuilding->getLength()/2),
			toPlace
		)) {
			throw BUILDING_COLLISION;
		}
		++it;
	}
	addBuilding(new Building(x, z));
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
