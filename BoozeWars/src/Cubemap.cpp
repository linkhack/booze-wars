#pragma once

#include "Cubemap.h"
#include "FreeImage.h"
#include <iostream>

Cubemap::Cubemap()
{
}

Cubemap::Cubemap(std::vector<std::string> faces)
{
	//Cubemap: right/left/top/bottom/back/front
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height;
	BYTE* data(0);
	//load textures
	for (unsigned int i = 0; i < faces.size(); ++i)
	{
		FREE_IMAGE_FORMAT texFormat = FreeImage_GetFileType(faces[i].c_str(), 0);
		if (texFormat == FIF_UNKNOWN) { std::cout << "Cubemap: Image format unknow" << std::endl;}
		FIBITMAP* texImage = FreeImage_Load(texFormat, faces[i].c_str());
		if (!texImage) { std::cout << "Cubemap: Image loading failed: " << faces[i].c_str()<<std::endl; }
		data = FreeImage_GetBits(texImage);
		width = FreeImage_GetWidth(texImage);
		height = FreeImage_GetHeight(texImage);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_BGRA, width, height,
			0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		FreeImage_Unload(texImage);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


Cubemap::~Cubemap()
{
}

void Cubemap::bind(unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}