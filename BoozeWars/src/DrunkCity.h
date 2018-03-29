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
	Enemy *enemiesAlive;
	std::list<Building> buildings;
	//std::vector<Enemy> enemiesAlive;
	int alive;
	Geometry *worldModel;
	
public:

	DrunkCity();
	~DrunkCity();
	DrunkCity(float x, float y, float z, std::shared_ptr<Material> material);

	DrunkCity(DrunkCity&&) = default;
	DrunkCity& operator=(DrunkCity&&) = default;

	void zeichne();
	void addEnemy(std::shared_ptr<Material> material);
};

