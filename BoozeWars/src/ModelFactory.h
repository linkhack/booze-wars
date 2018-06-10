#pragma once

#include "Model.h"

class ModelFactory
{
private:
	static ModelFactory* _instance;

	Model* defaultBuilding;
	Model* defaultEnemy;
	Model* defaultWeapon;

	ModelFactory();
	~ModelFactory();
	ModelFactory(ModelFactory const&) {};
	ModelFactory& operator=(ModelFactory const&) {};

public:
	static const int ModelFactory::DEFAULT_BUILDING = 1;
	static const int ModelFactory::DEFAULT_ENEMY = 2;
	static const int ModelFactory::DEFAULT_WEAPON = 3;

	Model* getModel(const int model);

	static ModelFactory* Instance();

};

