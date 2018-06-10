#pragma once
#include "Model.h"
#include "ModelFactory.h"
#include "Shader.h"

class Weapon
{
protected:
	Model* model;
	glm::mat4 modelMatrix;
	float time;
	float maxTime;
	float delay;

public:
	Weapon(glm::mat4 matrix, float time);
	~Weapon();
	bool draw(Shader* shader, float time);
	void implode();
};

