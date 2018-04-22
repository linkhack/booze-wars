#include "Texture.h"

Texture::Texture()
{
	_init = false;
}

void Texture::loadImage(std::string fileName)
{
	fileName = "assets/textures/" + fileName;
	glGenTextures(1, &_handle);
	glBindTexture(GL_TEXTURE_2D, _handle);
	int width, height;
	BYTE* data(0);

	// load Image
	FREE_IMAGE_FORMAT texFormat = FreeImage_GetFileType(fileName.c_str(), 0);
	if (texFormat == FIF_UNKNOWN) { std::cout << "Texture: Image format unknow" << std::endl; }
	FIBITMAP* texImage = FreeImage_Load(texFormat, fileName.c_str());
	if (!texImage) { std::cout << "Texture: Image loading failed: " << fileName.c_str() << std::endl; }
	data = FreeImage_GetBits(texImage);
	width = FreeImage_GetWidth(texImage);
	height = FreeImage_GetHeight(texImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, data);
	FreeImage_Unload(texImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}