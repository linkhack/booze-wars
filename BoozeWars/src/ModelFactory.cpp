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
	this->defaultBuilding = new Model("assets/objects/saloon.obj");
	this->defaultEnemy = new Model("assets/objects/jarracerbezaLow.obj");
	this->defaultWeapon = new Model("assets/objects/barrel.obj");
	this->jumpingEnemy = new Model("assets/objects/bottle.obj");
	this->streetLight = new Model("assets/objects/streetlamp.obj");
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
	case DEFAULT_WEAPON:
		return this->defaultWeapon;
		break;
	case JUMPING_ENEMY:
		return this->jumpingEnemy;
		break;
	case STREET_LIGHT:
		return this->streetLight;
		break;
	default:
		std::cout << "Model not founnd" << std::endl;
		return nullptr;
		break;
	}
}