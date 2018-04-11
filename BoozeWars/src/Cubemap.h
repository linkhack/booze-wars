#pragma once
#include <string>
#include <GL/glew.h>
#include <vector>


class Cubemap
{
private:
	GLuint textureID;
public:
	Cubemap(std::vector<std::string> faces);
	~Cubemap();
	void bind(unsigned int unit);

};

