#pragma once

#include "Model.h"

class ModelFactory
{
private:
	static ModelFactory* _instance;

	Model* defaultBuilding;

	ModelFactory();
	~ModelFactory();
	ModelFactory(ModelFactory const&) {};
	ModelFactory& operator=(ModelFactory const&) {};

public:
	static const int ModelFactory::DEFAULT_BUILDING = 1;

	Model* getModel(int model);

	static ModelFactory* Instance();

};

