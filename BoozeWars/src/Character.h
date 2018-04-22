#pragma once

#include <sstream>
#include "GL\glew.h"
#include "glm\glm.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H

#include "Shader.h"

class Character
{
private:
	GLuint _vboPositions;
	FT_Library ft;
	FT_Face face;
	GLuint _vao;
	std::map<char, GLuint> charToTexture;
	std::shared_ptr<Shader> shader;
	void renderText(const char *text, float x, float y);
	int windowWidth;
	int windowHeight;
	GLFWwindow *window;
public:
	Character(GLFWwindow *window);
	~Character();
	void setFontSize(int fontSize);
	void display(char* text);
};

