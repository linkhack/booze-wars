#pragma once
#include "Model.h"
#include "ModelFactory.h"
#include "Shader.h"
#include "ParticleGenerator.h"

class Weapon
{
protected:
	Model* model;
	glm::mat4 modelMatrix;
	float time;
	float ttl;
	float delay;
	float implodeTime;
	int direction;
	float movementspeed = 15.0f;
	float width;
	float length;
	bool hitted = false;
	float x;
	float z;
	float y;
	float rollRotate = 0.0f;
	ParticleGenerator* particleGenerator;

public:
	int static const Weapon::DELAY_FIRST = 0;
	int static const Weapon::DELAY_SECOND = 5;
	int static const Weapon::DELAY_THIRD = 10;
	int static const Weapon::DELAY_FOURTH = 15;
	int static const Weapon::DELAY_FIFTH = 20;

	Weapon(float x, float z, int direction, int delay);
	~Weapon();
	bool draw(Shader* shader, float time);
	void Weapon::drawParticle(Shader* particleShader);
	bool implode(float time);
	glm::mat4 Weapon::getModelMatrix();
	glm::mat2x2 getModel();
	void hit();
	bool isHitted();
};

