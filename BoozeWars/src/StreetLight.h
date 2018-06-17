#pragma once
#include "ModelFactory.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"

class StreetLight
{
private:
	Model* model;
	glm::mat4 modelMatrix;
	PointLight pointL;
public:
	StreetLight(glm::vec3 position);
	~StreetLight();
	void draw(Shader* shader);
	PointLight getPointLight();
};

