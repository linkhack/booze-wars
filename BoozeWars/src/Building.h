#pragma once
#include "Geometry.h"
#include "Enemy.h"
class Building
{
private:
	Geometry * model;
	int x;
	int y;
	float damage;
	int range;
public:
	Building();
	Building::Building(int x, int y, std::shared_ptr<Material> material);
	~Building();

	Building(Building&&) = default;
	Building& operator=(Building&&) = default;
	Building& operator=(const Building &) = default;

	int getX();
	int getY();
	float getDamage();
	float getRange();

	void draw();
};

