#pragma once
#include "Geometry.h"
#include "Street.h"
#include "Enemy.h"
#include "Building.h"
#include <glm\glm.hpp>
#include "Material.h"
#include "Skybox.h"


class DrunkCity
{
private:
	
	Street highway;
	//std::vector<Enemy> enemiesAlive;
	int alive;
	std::list<Enemy*> enemiesAlive;
	std::list<Building*> buildings;
	int limitBuildings;
	int citySizeX;
	int citySizeY;

	Enemy* getNearestEnemy(Building* building);
	void addBuilding(Building* building);
	
public:

	DrunkCity();
	~DrunkCity();
	DrunkCity(float x, float y, float z);

	DrunkCity(DrunkCity&&) = default;
	DrunkCity& operator=(DrunkCity&&) = default;

	void zeichne();
	void addEnemy(std::shared_ptr<Material> material);
	void fight();
	void walk(float dT);
	void placeBuilding(int x, int y, std::shared_ptr<Material> material);
};

