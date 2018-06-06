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
}

ModelFactory::~ModelFactory()
{
}

Model* ModelFactory::getModel(int model) {
	switch (model) {
	case DEFAULT_BUILDING:
		return this->defaultBuilding;
		break;
	}
}