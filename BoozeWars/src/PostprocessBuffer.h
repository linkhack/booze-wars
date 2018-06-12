#pragma once

#include "GL/glew.h"
#include "Shader.h"

class PostprocessBuffer
{
private:
	GLuint fboHandle;
	GLuint depthTexture;
	GLuint colorTexture;
	GLuint normalTexture;

	void setupTexture(GLuint* handle, int attachment);
	void setupDepthTexture(GLuint* handle);

	int width;
	int height;
	//Fullscreen quad rendering
	Shader* postprocessShader;
	GLuint quadVAO;
	
public:
	PostprocessBuffer(int width, int height);
	~PostprocessBuffer();

	void bindForReading();
	void bindForWriting();
};

