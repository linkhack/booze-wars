#pragma once
#include "Shader.h"
#include "Particle.h"
#include "Texture.h"
#include "ModelFactory.h"

class ParticleGenerator
{
private:
	Texture* texture;
	static const GLuint nr_particles = 500;
	Particle particles[nr_particles];
	GLuint VBO;
	GLuint VAO;
	GLuint positionBuffer;
	GLuint colorBuffer;

public:
	ParticleGenerator();
	~ParticleGenerator();

	void init();
	void draw(Shader* shader);
	void update(GLfloat dt, float x, float y);
};

