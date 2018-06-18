#pragma once

#include "Model.h"

class ModelFactory
{
private:
	static ModelFactory* _instance;

	Model* defaultBuilding;
	Model* defaultEnemy;
	Model* defaultWeapon;
	Model* jumpingEnemy;
	Model* streetLight;
	Model* school;

	Texture* weaponTexture;

	ModelFactory();
	~ModelFactory();
	ModelFactory(ModelFactory const&) {};
	ModelFactory& operator=(ModelFactory const&) {};

public:
	static const int ModelFactory::DEFAULT_BUILDING = 1;
	static const int ModelFactory::DEFAULT_ENEMY = 2;
	static const int ModelFactory::DEFAULT_WEAPON = 3;
	static const int ModelFactory::JUMPING_ENEMY = 4;
	static const int ModelFactory::STREET_LIGHT = 5;
	static const int ModelFactory::SCHOOL = 6;

	Model* getModel(const int model);

	static ModelFactory* Instance();

};

