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
	wave = Wave(std::list<wavetuple>({ wavetuple(Enemy::DEFAULT_ENEMY,10,2,1),wavetuple(Enemy::JUMPING_ENEMY,10,0.5,1),wavetuple(Enemy::DEFAULT_ENEMY,5,0.2,1),wavetuple(Enemy::JUMPING_ENEMY,10,1,0.1),wavetuple(Enemy::DEFAULT_ENEMY,20,0.25,1) }));
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
		//(*it)->drawShadows(shader);
	}

	for (std::list<Building*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		(*it)->drawShadows(shader);
	}
}
void DrunkCity::zeichne(Shader* shader, float time)
{

	for (std::list<Enemy*>::iterator it = enemiesAlive.begin(); it != enemiesAlive.end(); ++it)
	{
		Enemy* iteratingEnemy = *it;
		iteratingEnemy->draw(shader);
	}

	for (std::list<Building*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		Building* iteratingBuilding = *it;
		iteratingBuilding->draw(shader, time);
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

void DrunkCity::addEnemy(float dT)
{
	int type = wave.spawnEnemy(dT);
	Enemy* enemyToAdd;
	switch (type)
	{
	case Enemy::DEFAULT_ENEMY:
		enemyToAdd = new Enemy(highway);
		break;
	case Enemy::JUMPING_ENEMY:
		enemyToAdd = new JumpingEnemy(highway);
		break;
	default:
		return;
		break;
	}
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

bool DrunkCity::waveIsFinished()
{
	return wave.waveIsFinished();
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
		/*Enemy* enemy = getNearestEnemy(building);
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
		*/

		building->shoot(dT);
		detectCollision();
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
		//iterEnemy->applyForce(iterEnemy->getDesiredDirection());
		iterEnemy->applyDrivingForce();
		std::list<Enemy*>::iterator otherEnemyIt = enemiesAlive.begin();
		while (otherEnemyIt != enemiesAlive.end())
		{
			if (otherEnemyIt != it) {
				Enemy* otherEnemy = *otherEnemyIt;
				iterEnemy->applyForce(-0.2f*9.6f*iterEnemy->getDirectionVector(*otherEnemy) / iterEnemy->getDistanceSquared(*otherEnemy));
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

	addBuilding(new Building(x, z, getStreetDirection(x,z, Building::width, Building::length)));
}

int DrunkCity::getStreetDirection(float x, float z, int width, int length)
{
	float xb = x + width;
	float zb = z + length;
	float x1 = this->highway->getPart1()[0][0];
	float z1 = this->highway->getPart1()[0][1];
	float xb1 = this->highway->getPart1()[1][0];
	float zb1 = this->highway->getPart1()[1][1];
	float x2 = this->highway->getPart2()[0][0];
	float z2 = this->highway->getPart2()[0][1];
	float xb2 = this->highway->getPart2()[1][0];
	float zb2 = this->highway->getPart2()[1][1];
	float x3 = this->highway->getPart3()[0][0];
	float z3 = this->highway->getPart3()[0][1];
	float xb3 = this->highway->getPart3()[1][0];
	float zb3 = this->highway->getPart3()[1][1];

	float mx1 = std::min(abs(x-x1), abs(x-xb1));
	float mxb1 = std::min(abs(xb-x1), abs(xb-xb1));
	float mz1 = std::min(abs(z-z1), abs(z-zb1));
	float mzb1 = std::min(abs(zb-z1), abs(zb-zb1));
	float m1 = std::min(mz1, mzb1);
	float mx2 = std::min(abs(x - x2), abs(x - xb2));
	float mxb2 = std::min(abs(xb - x2), abs(xb - xb2));
	float mz2 = std::min(abs(z - z2), abs(z - zb2));
	float mzb2 = std::min(abs(zb - z2), abs(zb - zb2));
	float m2 = std::min(mx2, mxb2);
	float mx3 = std::min(abs(x - x3), abs(x - xb3));
	float mxb3 = std::min(abs(xb - x3), abs(xb - xb3));
	float mz3 = std::min(abs(z - z3), abs(z - zb3));
	float mzb3 = std::min(abs(zb - z3), abs(zb - zb3));
	float m3 = std::min(mz3, mzb3);

	if (m1 < m2 && m1 < m3) {
		return this->getDirection(mx1, mz1, mxb1, mzb1);
	}
	if (m2 < m1 && m2 < m3) {
		return this->getDirection(mx2, mz2, mxb2, mzb2);
	}
	return this->getDirection(mx3, mz3, mxb3, mzb3);
}

void DrunkCity::detectCollision() {
	for (std::list<Building*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		Building* building = *it;
		std::list<Weapon*> activeWeapons = building->getWeapons();
		for (std::list<Weapon*>::iterator it = activeWeapons.begin(); it != activeWeapons.end(); ++it)
		{
			Weapon* weapon = *it;
			for (std::list<Enemy*>::iterator it = enemiesAlive.begin(); it != enemiesAlive.end();)
			{
				Enemy* enemy = *it;
				if (enemyCollision(weapon, enemy)) {
					enemy->hit(building->getDamage());
					if (enemy->getHP() <= 0) {
						limitBuildings += 0.1;
						it = enemiesAlive.erase(it);
						enemy->selfDestruct();
						delete enemy;
						enemy = NULL;
					}
					else {
						it++;
					}
					weapon->hit();
				}
				else {
					it++;
				}
			}
		}
	}
}

bool DrunkCity::enemyCollision(Weapon* weapon, Enemy* enemy) {
	if (weapon->getModel()[0][0] > enemy->getModel()[1][0] ||
		weapon->getModel()[0][1] > enemy->getModel()[1][1] ||
		weapon->getModel()[1][0] < enemy->getModel()[0][0] ||
		weapon->getModel()[1][1] < enemy->getModel()[0][1]) {
		return false;
	}
	return true;
}

int DrunkCity::getDirection(float mx, float mz, float mxb, float mzb)
{
	// front face is the nearest
	if (mz < mzb && (mx + mxb < Building::width || mz < mx && mz < mxb) && mz + mzb >= Building::length) {
		return 2;
	}
	// right face is the nearest
	if (mx < mxb && (mz + mzb < Building::width || mx < mzb && mx < mxb) && mx + mxb >= Building::width) {
		return 3;
	}
	// back face is the nearest
	if (mzb < mz && (mx + mxb < Building::width || mzb < mx && mzb < mxb) && mzb + mz >= Building::length) {
		return 0;
	}
	// left face is the nearest
	return 1;
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
