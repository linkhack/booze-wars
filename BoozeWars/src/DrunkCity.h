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
#include "Wall.h"
#include "Frustum.h"
#include <PhysX/PxPhysicsAPI.h>
using namespace physx;

class DrunkCity
{
private:
	
	//std::vector<Enemy> enemiesAlive;
	int alive;
	std::list<Enemy*> enemiesAlive;
	std::list<Building*> buildings;
	std::list<Wall*> wallList;
	float limitBuildings;
	int limitWalls;
	int citySizeX;
	int citySizeZ;
	int hp;
	PxPhysics* gPhysicsSDK;
	PxScene* gScene;
	Wave* wave;
	Frustum* frustum;

	void addBuilding(Building* building);
	void addWall(Wall* wall);
	bool isColliding(glm::mat2x2 placed, glm::mat2x2 toPlace);
	int DrunkCity::getDirection(float mx, float mz, float mxb, float mzb);
	void DrunkCity::detectCollision();
	bool DrunkCity::enemyCollision(Weapon* weapon, Enemy* enemy);	
		
public:
	Street* highway;

	DrunkCity();
	~DrunkCity();
	DrunkCity(float x, float y, float z);

	DrunkCity(DrunkCity&&) = default;
	DrunkCity& operator=(DrunkCity&&) = default;

	int getHP();
	void drawShadows(Shader & shader);
	void zeichne(Shader* shader, float time);
	void DrunkCity::drawParticles(Shader* particleShader);
	void fight(float dT);
	void walk();
	void calculateForces();
	void placeBuilding(float x, float z, int direction);
	void placeWall(float x, float z,std::shared_ptr<Material> material);
	bool hasMinOneBuildings();
	int getBuildingsLeft();
	void addEnemy(float dT);
	PxScene* initPhysics(PxPhysics* gPhysicsSDK);
	bool waveIsFinished();
	int getStreetDirection(float x, float z, int width, int length);
	bool hasNextWave();
	glm::vec3 getStreetLightPos();
	void updateFrustum(myCamera& camera);
};