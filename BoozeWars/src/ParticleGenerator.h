#pragma once
#include "Shader.h"
#include "Particle.h"
#include "Texture.h"
#include "ModelFactory.h"

class ParticleGenerator
{
private:
	Texture* texture;
	GLuint nr_particles = 500;
	std::vector<Particle> particles;
	GLuint VAO;

public:
	ParticleGenerator();
	~ParticleGenerator();

	void init();
	void draw(Shader* shader);
	void update(GLfloat dt, float x, float y);
};

