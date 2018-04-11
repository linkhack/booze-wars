#pragma once

#include "Cubemap.h"
#include "FreeImage.h"


Cubemap::Cubemap(std::vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	//load textures
	
}


Cubemap::~Cubemap()
{
}
