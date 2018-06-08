#pragma once
#include "Mesh.h"

Mesh::Mesh() {

}

Mesh::~Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(Vertex),
		&vertices.front(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(GLuint),
		&indices.front(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::draw(std::shared_ptr<Shader> shader)
{
	shader->use();
	int diffuseNr = 0;
	GLuint specularNr = 5;
	shader->setUniform("materialCoefficients", glm::vec3(0.2f, 0.8f, 0.1f));
	shader->setUniform("specularAlpha", 20.0f);
	/*
	GLint backgroundlocation = glGetUniformLocation(shader.getID(), "colorTexture");
	GLint specularlocation = glGetUniformLocation(shader.getID(), "colorTexture");
	GLint lightColorLoc = glGetUniformLocation(shader.getID(), "lightColor");
	GLint objectColorLoc = glGetUniformLocation(shader.getID(), "objectColor");
	*/
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		std::string name = this->textures[i].type;
		GLuint textid = textures[i].id;

		if (name == "texture_diffuse")
		{
			glActiveTexture(GL_TEXTURE0 + diffuseNr);
			glBindTexture(GL_TEXTURE_2D, textid);
			shader->setUniform("diffuseTexture", diffuseNr);
		}
		else if (name == "texture_specular")
		{

			//shader->setUniform("specularTexture", specularNr);
			//textureUnit = specularNr;
		}

	}


	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
