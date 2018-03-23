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
	Geometry worldModel;
	Street highway;
	std::list<Enemy> enemiesAlive;
	std::list<Building> buildings;
	
public:
	DrunkCity();
	~DrunkCity();
	DrunkCity(float x, float y, float z, std::shared_ptr<Material> material);

	void draw();
};

