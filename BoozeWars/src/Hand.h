#pragma once

#include <glm\glm.hpp>

#include "Model.h"
#include "ModelFactory.h"
#include "Shader.h"

class Hand
{
private:
	Model* model;
	glm::mat4 modelMatrix;
	float rotation;
	bool up;
public:
	Hand();
	~Hand();
	void draw(Shader* shader, glm::mat4 modelMatrix);
	void rotate(float time);
};

