#pragma once

#include <sstream>
#include "GL\glew.h"
#include "glm\glm.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H

#include "Shader.h"

struct CharacterData
{
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint advance;
};
class Character
{
private:
	GLuint _vboPositions;
	FT_Library ft;
	FT_Face face;
	GLuint _vao;
	std::map<GLchar, CharacterData> charToTexture;
	std::shared_ptr<Shader> shader;
	int windowWidth;
	int windowHeight;
	GLFWwindow *window;
	GLuint allWhite;
public:
	Character(GLFWwindow *window, int fontSize);
	~Character();
	void setFontSize(int fontSize);
	void renderText(const char *text, float x, float y, GLfloat scale, glm::vec3 color);
};

