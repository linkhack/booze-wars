#pragma once
#include "Geometry.h"
#include "Street.h"
#include "Enemy.h"
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
	
	
	
public:

	DrunkCity();
	~DrunkCity();
	DrunkCity(float x, float y, float z);

	DrunkCity(DrunkCity&&) = default;
	DrunkCity& operator=(DrunkCity&&) = default;

	int getHP();
	void drawShadows(Shader & shader);
	void zeichne(std::shared_ptr<Shader> shader);
	void fight(float dT);
	void walk();
	void calculateForces();
	void placeBuilding(float x, float z);
	bool hasMinOneBuildings();
	int getBuildingsLeft();
	void addEnemy(std::shared_ptr<Geometry> model);
	PxScene* initPhysics(PxPhysics* gPhysicsSDK);

};

