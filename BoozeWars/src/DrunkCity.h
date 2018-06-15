#pragma once
#include "Geometry.h"
#include "Street.h"
#include "Enemy.h"
#include "JumpingEnemy.h"
#include "Building.h"
#include <glm\glm.hpp>
#include "Material.h"
#include "Skybox.h"
#include "Wave.h"
#include <PhysX/PxPhysicsAPI.h>
using namespace physx;

class DrunkCity
{
private:
	
	std::shared_ptr<Street> highway;
	//std::vector<Enemy> enemiesAlive;
	int alive;
	std::list<Enemy*> enemiesAlive;
	std::list<Building*> buildings;
	float limitBuildings;
	int citySizeX;
	int citySizeZ;
	int hp;
	PxPhysics* gPhysicsSDK;
	PxScene* gScene;

	Enemy* getNearestEnemy(Building* building);
	void addBuilding(Building* building);
	bool isColliding(glm::mat2x2 placed, glm::mat2x2 toPlace);
	int DrunkCity::getStreetDirection(float x, float z, int width, int length);
	int DrunkCity::getDirection(float mx, float mz, float mxb, float mzb);
	void DrunkCity::detectCollision();
	bool DrunkCity::enemyCollision(Weapon* weapon, Enemy* enemy);	
	
	
public:

	DrunkCity();
	~DrunkCity();
	DrunkCity(float x, float y, float z);

	DrunkCity(DrunkCity&&) = default;
	DrunkCity& operator=(DrunkCity&&) = default;

	int getHP();
	void drawShadows(Shader & shader);
	void zeichne(Shader* shader, float time);
	void fight(float dT);
	void walk();
	void calculateForces();
	void placeBuilding(float x, float z);
	bool hasMinOneBuildings();
	int getBuildingsLeft();
	void addEnemy(const int modelNr=ModelFactory::DEFAULT_ENEMY);
	PxScene* initPhysics(PxPhysics* gPhysicsSDK);

};

