#pragma once
#include "Geometry.h"
#include "Street.h"
#include "Enemy.h"
#include "Building.h"
#include <glm\glm.hpp>
#include "Material.h"
#include "Skybox.h"
#include "Wave.h"


class DrunkCity
{
private:
	
	std::shared_ptr<Street> highway;
	//std::vector<Enemy> enemiesAlive;
	int alive;
	std::list<Enemy*> enemiesAlive;
	std::list<Building*> buildings;
	int limitBuildings;
	int citySizeX;
	int citySizeZ;
	int hp;

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
	void zeichne();
	void fight(float dT);
	void walk(float dT);
	void placeBuilding(int x, int z, std::shared_ptr<Material> material);
	bool hasMinOneBuildings();
	int getBuildingsLeft();
	void addEnemy(std::shared_ptr<Material> material);

};

