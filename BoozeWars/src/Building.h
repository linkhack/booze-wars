#pragma once
#include "Geometry.h"
#include "Enemy.h"
class Building
{
private:
	Geometry * model;
	int x;
	int z;
	float damage;//demage per second
	int range;
public:
	Building();
	Building::Building(int x, int z, std::shared_ptr<Material> material);
	~Building();

	Building(Building&&) = default;
	Building& operator=(Building&&) = default;
	Building& operator=(const Building &) = default;

	int getX();
	int getZ();
	static int getWidth();
	static int getLength();
	float getDamage();
	int getRange();

	void draw();
};

