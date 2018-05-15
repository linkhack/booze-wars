#pragma once
#include "GL/glew.h"
class ShadowMapSimple
{
private:
	GLuint _fbo;	//FBO_ID
	GLuint _shadowMap; //texture_ID
public:
	ShadowMapSimple();
	bool init(unsigned int window_width, unsigned int window_height);
	void bindForWriting();
	void bindForReading();
	~ShadowMapSimple();
};

