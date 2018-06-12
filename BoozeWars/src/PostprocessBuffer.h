#pragma once

#include "GL/glew.h"
#include "Shader.h"

class PostprocessBuffer
{
private:
	GLuint fboHandle;
	GLuint depthbuffer;
	GLuint depthTexture;
	GLuint colorTexture;
	GLuint normalTexture;

	void setupTexture(GLuint* handle, int attachment, GLenum format);
	void setupDepthTexture(GLuint* handle);

	int width;
	int height;
	//Fullscreen quad rendering
	Shader* postprocessShader;
	GLuint quadVAO;
	GLuint positionVBO;
	GLuint uvVBO;
	
public:
	PostprocessBuffer(int width, int height, float nearz, float farz);
	~PostprocessBuffer();

	void renderToScreen();
	void bindForWriting();
};

