#include "StreetLight.h"

StreetLight::StreetLight(glm::vec3 position)
{
	ModelFactory* factory = ModelFactory::Instance();
	model = factory->getModel(ModelFactory::STREET_LIGHT);
	modelMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::vec3 lightPos = glm::vec3(position.x, 3.0f, position.z);
	pointL = PointLight(glm::vec3(0, 10.0f, 0), lightPos, glm::vec3(1, 0, 1));
}

StreetLight::~StreetLight()
{
}

void StreetLight::draw(Shader* shader)
{
	model->draw(shader, modelMatrix);
}

PointLight StreetLight::getPointLight()
{
	return pointL;
}