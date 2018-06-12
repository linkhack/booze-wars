#include "PostprocessBuffer.h"
#include <iostream>


PostprocessBuffer::PostprocessBuffer(int width, int height)
{
	//set constants
	this->width = width;
	this->height = height;
	postprocessShader = new Shader("postprocess.vert","postprocess.frag");

	//full screen quad
	float vertices[] =
		{
			-1.0f, -1.0f, 0.1f,
			1.0f, -1.0f, 0.1f,
			-1.0f,  1.0f, 0.1f,
			1.0f,1.0f,0.1f
		};
	
	float uv[] =
		{
			0.0f,0.0f,
			1.0f,0.0f,
			0.0f,1.0f,
			1.0f,1.0f
		};
	// create VAO
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	// create positions VBO
	glGenBuffers(1, &positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// create uvs VBO
	glGenBuffers(1, &uvVBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

	// bind uvs to location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Create Framebuffer
	glGenFramebuffers(1, &fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	
	//Attach depth buffer
	glGenRenderbuffers(1, &depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);
	//Setup textures
	setupDepthTexture(&depthTexture); //sets up the depth buffer too
	setupTexture(&colorTexture, 0, GL_RGBA);
	setupTexture(&normalTexture, 1,GL_RGBA);
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "Framebuffer problem" << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
}


PostprocessBuffer::~PostprocessBuffer()
{
}

void PostprocessBuffer::setupTexture(GLuint* handle, int attachment, GLenum format)
{
	glGenTextures(1, handle);
	glBindTexture(GL_TEXTURE_2D, *handle);
	glTexImage2D(GL_TEXTURE_2D, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, 0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+attachment, *handle, 0);
}

void PostprocessBuffer::setupDepthTexture(GLuint* handle)
{
	glGenTextures(1, handle);
	glBindTexture(GL_TEXTURE_2D, *handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *handle, 0);
}

void PostprocessBuffer::bindForWriting() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
}

void PostprocessBuffer::renderToScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	postprocessShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,colorTexture);
	postprocessShader->setUniform("colorInformation", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalTexture);
	postprocessShader->setUniform("normalInformation", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	postprocessShader->setUniform("depthInformation", 2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	
}