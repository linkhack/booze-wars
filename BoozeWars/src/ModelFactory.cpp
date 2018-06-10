#include <stddef.h>
#include "ModelFactory.h"

ModelFactory* ModelFactory::_instance = NULL;

ModelFactory* ModelFactory::Instance()
{
	if (!_instance) {
		_instance = new ModelFactory;
	}

	return _instance;
}

ModelFactory::ModelFactory()
{
	this->defaultBuilding = new Model("assets/objects/saloon2.obj");
	this->defaultEnemy = new Model("assets/objects/jarracerbezaLow.obj");
}

ModelFactory::~ModelFactory()
{
}

Model* ModelFactory::getModel(const int model) {
	switch (model) {
	case DEFAULT_BUILDING:
		return this->defaultBuilding;
		break;
	case DEFAULT_ENEMY:
		return this->defaultEnemy;
		break;
	default:
		std::cout << "Model not founnd" << std::endl;
		return nullptr;
		break;
	}
}