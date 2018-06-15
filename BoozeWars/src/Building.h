#pragma once
#include "Geometry.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Model.h"
#include "ModelFactory.h"
#include <cstdlib>
class Building
{
private:
	Model* model;
	int x;
	int z;
	int direction;
	float damage;//demage per second
	glm::mat4 modelMatrix;
	std::list<Weapon*> activeWeapons;
	float shootIntervall;
	float time;
public:
	int static const Building::width = 20;
	int static const Building::length = 28;
	int static const Building::range = 70;
	Building();
	Building::Building(float x, float z, int streetDirection);
	~Building();

	Building(Building&&) = default;
	Building& operator=(Building&&) = default;
	Building& operator=(const Building &) = default;

	int getX();
	int getZ();
	static int getWidth();
	static int getLength();
	static int getRange();
	float getDamage();

	void drawShadows(Shader& shader);
	void draw(Shader* shader, float time);
	void shoot(float time);
	std::list<Weapon*> getWeapons();
};

