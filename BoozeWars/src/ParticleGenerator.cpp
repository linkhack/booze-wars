#include "ParticleGenerator.h"



ParticleGenerator::ParticleGenerator()
{
	this->init();
}


ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::init()
{
	// Set up mesh and attribute properties
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->nr_particles; ++i)
		this->particles.push_back(Particle());
}

void ParticleGenerator::draw(Shader* shader)
{
	for (Particle particle : this->particles)
	{
		shader->use();
		if (particle.Life > 0.0f)
		{
			glm::vec3 position = particle.Position + particle.Modifier;
			shader->setUniform("position", position);
			shader->setUniform("color", particle.Color);
			/*
			this->texture->bind(texture->id);
			
			glActiveTexture(GL_TEXTURE0 + texture->id);
			glBindTexture(GL_TEXTURE_2D, texture->id);
			shader->setUniform("texture", texture->id);
			*/
			
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
}

void ParticleGenerator::update(GLfloat dt, float x, float z)
{
	for (GLuint i = 0; i < this->nr_particles; ++i)
	{
		Particle &p = this->particles[i];
		p.Life -= dt; // reduce life
		if (p.Life > 0.0f && p.Color.a > 0)
		{
			if (p.Direction == glm::vec3(0.0f)) {
				p.Direction = glm::vec3(rand() % 2, rand() % 2, rand() % 2);
			}

			for (int i = 0; i < 3; i++) {
				float diff = 0.5 - 0.0;
				float random = (((float)rand() / RAND_MAX) * diff) + 0.0;
				//float random = (float)(rand() % 411)/1000;
				if (p.Direction[i] == 0) {
					p.Modifier[i] += random * (rand() % 2);
				}
				else {
					p.Modifier[i] -= random * (rand() % 2);
				}
			}
			p.Position = glm::vec3(x, 2.0f, z);
			p.Color.a -= dt * 1.0f;
		}
	}
}
