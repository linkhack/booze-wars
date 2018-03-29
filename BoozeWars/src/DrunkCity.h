#pragma once
#include "Geometry.h"
#include "Street.h"
#include "Enemy.h"
#include "Building.h"
#include <glm\glm.hpp>
#include "Material.h"


class DrunkCity
{
private:
	
	Street highway;
	//std::vector<Enemy> enemiesAlive;
	int alive;
	std::list<Enemy*> enemiesAlive;
	std::list<Building*> buildings;
	int limitBuildings;
	Geometry *worldModel;
	int citySizeX;
	int citySizeY;

	Enemy* getNearestEnemy(Building* building);
	
public:

	DrunkCity();
	~DrunkCity();
	DrunkCity(float x, float y, float z, std::shared_ptr<Material> material);

	DrunkCity(DrunkCity&&) = default;
	DrunkCity& operator=(DrunkCity&&) = default;

	void zeichne();
	void addEnemy(std::shared_ptr<Material> material);
	void addBuilding(int x, int y, std::shared_ptr<Material> material);
	void fight();
};

