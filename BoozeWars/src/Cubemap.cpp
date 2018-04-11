
#include "Cubemap.h"



Cubemap::Cubemap()
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	
}


Cubemap::~Cubemap()
{
}
