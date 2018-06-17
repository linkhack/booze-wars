#include "ParticleGenerator.h"



ParticleGenerator::ParticleGenerator()
{
	this->init();
}


ParticleGenerator::~ParticleGenerator()
{
	//delete particles;
}

void ParticleGenerator::init()
{
	// Set up mesh and attribute properties
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

	//position buffer
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->nr_particles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	//color buffer
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->nr_particles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->nr_particles; ++i)
		particles[i] = Particle();

}

void ParticleGenerator::draw(Shader* shader)
{
	shader->use();
	glm::vec3 particlePosition[nr_particles];
	glm::vec4 particleColor[nr_particles];
	int i = 0;
	for (Particle particle : this->particles)
	{
		
		if (particle.Life > 0.0f&& particle.Color.a>0.0f)
		{
			glm::vec3 position = particle.Position + particle.Modifier;
			particlePosition[i] = position;
			particleColor[i] = particle.Color;
			/*
			this->texture->bind(texture->id);
			
			glActiveTexture(GL_TEXTURE0 + texture->id);
			glBindTexture(GL_TEXTURE_2D, texture->id);
			shader->setUniform("texture", texture->id);
			*/
		}
		++i;
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particlePosition), NULL, GL_STREAM_DRAW); 
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particlePosition), particlePosition);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleColor), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particleColor), particleColor);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(
		0, // attribute. No particular reason for 0, but must match the layout in the shader.
		4, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glVertexAttribPointer(
		1, // attribute. No particular reason for 1, but must match the layout in the shader.
		3, // size : x + y + z + size => 4
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(
		2, // attribute. No particular reason for 1, but must match the layout in the shader.
		4, // size : r + g + b + a => 4
		GL_FLOAT, // type
		GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0, // stride
		(void*)0 // array buffer offset
	);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->nr_particles);
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
				float phi = rand()%360 * glm::pi<float>()/180.0f;
				float theta = rand()%180 * glm::pi<float>() / 180.0f;
				p.Direction = glm::vec3(glm::sin(theta)*glm::cos(phi), glm::cos(theta),glm::sin(theta)*glm::sin(phi));
			}

			for (int i = 0; i < 3; i++) {
				float diff = 0.3 - 0.0;
				float random = (((float)rand() / RAND_MAX) * diff) + 0.0;
				p.Modifier[i] += random * (rand() % 2)*p.Direction[i];
				//float random = (float)(rand() % 411)/1000;
				
			}
			p.Position = glm::vec3(x, 2.0f, z);
			p.Color.a -= dt * 1.0f;
		}
	}
}
