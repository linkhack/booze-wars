#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include "Shader.h"
#include "Cubemap.h"

class Skybox
{
private:
	std::vector<glm::vec3> vertices;
	std::shared_ptr<Shader> shader;
	GLuint _vao;
	GLuint _vboPositions;
	Cubemap texture;
public:
	Skybox(std::shared_ptr<Shader> shader,std::vector<std::string> faces);
	~Skybox();
	void draw();
};

