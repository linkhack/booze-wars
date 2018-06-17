#pragma once
#include "Utils.h"

class Particle
{
private:
public:
	glm::vec3 Position;
	glm::vec3 Modifier;
	glm::vec4 Color;
	glm::vec3 Direction;
	GLfloat Life;

	Particle(): Position(0.0f), Modifier(0.0f), Color(0.545f, 0.271f, 0.075f, 1.0f), Direction(0.0f), Life(1.0f) { }
	~Particle();
};

